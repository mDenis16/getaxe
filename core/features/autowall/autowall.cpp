#include "../features.hpp"

#define MAX_COORD_FLOAT		16'384.f			// max world coordinates
#define MIN_COORD_FLOAT		-MAX_COORD_FLOAT	// min world coordinates
#define MAX_DISTANCE		8'192.f				// max bullet distance, etc
#define MAX_FOV				180.f				// max field of view
#define MAX_DAMAGE			500.f				// max weapons damage
#define MAX_WEAPONS			64					// max number of weapons available

int autowall::get_damage ( player_t * from, const vec3_t & start, const vec3_t & end, FireBulletData_t & data ) {

	int dmg = 0;

	data.vecPosition = start;
	data.filter.skip = from;
	vec3_t angle = math::calc_angle ( start, end );

	data.vecDir = math::angle_vector ( angle );
	data.vecDir.normalize_in_place ( );

	auto weapon = from->active_weapon ( );

	if ( !weapon )
		return 0;

	if ( SimulateFireBullet ( from, weapon, data ) )
		dmg = static_cast< int >( data.flCurrentDamage );

	return dmg;
}

bool VectortoVectorVisible ( vec3_t src, vec3_t point ) {
	trace_t TraceInit;
	autowall::TraceLine ( src, point, MASK_SOLID, local_player::m_data.pointer, &TraceInit );
	trace_t Trace;
	autowall::TraceLine ( src, point, MASK_SOLID, TraceInit.entity, &Trace );

	if ( Trace.flFraction == 1.0f || TraceInit.flFraction == 1.0f )
		return true;

	return false;
}

bool autowall::can_hit_float_point ( const vec3_t & point, const vec3_t & source ) {

	vec3_t angView;
	vec3_t vecDirection;
	vec3_t vecDelta = point - source;

	csgo::in_trace = true;
	// setup data
	FireBulletData_t data;

	data.vecPosition = source;

	math::vector_angles ( vecDelta, angView );
	vecDirection = math::angle_vector ( angView );
	vecDirection.normalize_in_place ( );
	data.vecDir = vecDirection;

	auto pWeapon = local_player::m_data.pointer->active_weapon ( );

	if ( pWeapon == nullptr )
		return false;

	if ( !pWeapon->get_weapon_data ( ) )
		return false;

	auto weaponData = pWeapon->get_weapon_data ( );

	data.flCurrentDamage = ( float ) weaponData->iDamage;
	float trace_length_remaining = weaponData->flRange;
	vec3_t end = data.vecPosition + ( data.vecDir * trace_length_remaining );
	TraceLine ( data.vecPosition, end, MASK_SHOT | CONTENTS_HITBOX, local_player::m_data.pointer, &data.enterTrace );

	csgo::in_trace = false;
	if ( VectortoVectorVisible ( data.vecPosition, point ) )
		return true;

	return false;
}
void autowall::ScaleDamage ( int iHitGroup, player_t * pEntity, float flWeaponArmorRatio, float & flDamage ) {
	bool bHeavyArmor = pEntity->has_heavy_armor ( );
	int iArmor = pEntity->armor ( );

	switch ( iHitGroup ) {
	case hitgroup_head:
		flDamage *= bHeavyArmor ? 2.0f : 4.0f;
		break;
	case hitgroup_stomach:
		flDamage *= 1.25f;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		flDamage *= 0.75f;
		break;
	}

	// check is armored
	if ( iArmor > 0 && ( ( iHitGroup == hitgroup_head && pEntity->has_helmet ( ) ) || ( iHitGroup >= hitgroup_generic && iHitGroup <= hitgroup_rightarm ) ) ) {
		float flModifier = 1.0f, flArmorBonusRatio = 0.5f, flArmorRatio = flWeaponArmorRatio * 0.5f;

		if ( bHeavyArmor ) {
			flArmorBonusRatio = 0.33f;
			flArmorRatio *= 0.5f;
			flModifier = 0.33f;
		}

		float flNewDamage = flDamage * flArmorRatio;

		if ( bHeavyArmor )
			flNewDamage *= 0.85f;

		if ( ( ( flDamage - flDamage * flArmorRatio ) * ( flModifier * flArmorBonusRatio ) ) > iArmor )
			flNewDamage = flDamage - iArmor / flArmorBonusRatio;

		flDamage = flNewDamage;
	}
}

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/util_shared.cpp#L687
void autowall::ClipTraceToPlayers ( const vec3_t & vecAbsStart, const vec3_t & vecAbsEnd, unsigned int fMask, trace_filter * pFilter, trace_t * pTrace ) {
	trace_t trace = { };
	float flSmallestFraction = pTrace->flFraction;

	ray_t ray = { };
	ray.initialize ( vecAbsStart, vecAbsEnd );

	for ( int i = 1; i <= interfaces::globals->max_clients; i++ ) {
		player_t * pEntity = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( i ) );

		if ( pEntity == nullptr || !pEntity->is_alive ( ) || pEntity->dormant ( ) )
			continue;

		if ( pFilter != nullptr && pFilter->ShouldHitEntity ( pEntity, fMask ) == false )
			continue;


		if ( !pEntity->collideable ( ) )
			continue;

		// get bounding box
		const vec3_t vecMin = pEntity->collideable ( )->mins ( );
		const vec3_t vecMax = pEntity->collideable ( )->maxs ( );

		// calculate world space center
		const vec3_t vecCenter = ( vecMax + vecMin ) * 0.5f;
		const vec3_t vecPosition = vecCenter + pEntity->origin ( );

		vec3_t vecTo = vecPosition - vecAbsStart;
		vec3_t vecDir = vecAbsEnd - vecAbsStart;
		float flLength = vecDir.normalize_in_place ( );

		const float flRangeAlong = vecDir.dot_product ( vecTo );
		float flRange = 0.0f;

		// calculate distance to ray
		// off start point
		if ( flRangeAlong < 0.0f )
			flRange = -vecTo.length ( );
		// off end point
		else if ( flRangeAlong > flLength )
			flRange = -( vecPosition - vecAbsEnd ).length ( );
		// within ray bounds
		else {
			vec3_t vecRay = vecPosition - ( vecDir * flRangeAlong + vecAbsStart );
			flRange = vecRay.length ( );
		}

		if ( flRange < 0.0f || flRange > 60.0f )
			continue;

		interfaces::trace_ray->clip_ray_to_entity ( ray, fMask | CONTENTS_HITBOX, pEntity, &trace );

		if ( trace.flFraction < flSmallestFraction ) {
			// we shortened the ray - save off the trace
			*pTrace = trace;
			flSmallestFraction = trace.flFraction;
		}
	}
}

// tried to rebuild this but crashing with CBaseDoor???
bool autowall::IsBreakableEntity ( player_t * pEntity ) {
	if ( pEntity == nullptr || pEntity->index ( ) == 0 )
		return false;

	// backup original takedamage value
	const int iOldTakeDamage = pEntity->get_take_damage ( );
	const c_client_class * pClientClass = pEntity->client_class ( );

	if ( pClientClass == nullptr )
		return false;

	// force set DAMAGE_YES for certain breakable entities (as props, doors, etc)
	if ( pClientClass->class_id == class_ids::CBreakableSurface || pClientClass->class_id == class_ids::CBaseDoor || pClientClass->class_id == class_ids::CFuncBrush )
		pEntity->get_take_damage ( ) = 2;

	using IsBreakableEntityFn = bool ( __thiscall * )( player_t * );
	static IsBreakableEntityFn oIsBreakableEntity = oIsBreakableEntity = ( IsBreakableEntityFn ) ( utilities::pattern_scan ( "client.dll", ( "55 8B EC 51 56 8B F1 85 F6 74 68" ) ) );

	// restore original take damage
	pEntity->get_take_damage ( ) = iOldTakeDamage;
	return oIsBreakableEntity ( pEntity );
}

bool autowall::TraceToExit ( trace_t * pEnterTrace, trace_t * pExitTrace, vec3_t vecPosition, vec3_t vecDirection ) {
	float flDistance = 0.0f;
	int iStartContents = 0;

	while ( flDistance <= 90.0f ) {
		// add extra distance to our ray
		flDistance += 4.0f;

		// multiply the direction vec3_t to the distance so we go outwards, add our position to it
		vec3_t vecStart = vecPosition + vecDirection * flDistance;

		if ( !iStartContents )
			iStartContents = interfaces::trace_ray->get_point_contents ( vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );

		const int iCurrentContents = interfaces::trace_ray->get_point_contents ( vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );

		if ( !( iCurrentContents & MASK_SHOT_HULL ) || iCurrentContents & CONTENTS_HITBOX && iCurrentContents != iStartContents ) {
			// setup our end position by deducting the direction by the extra added distance
			const vec3_t vecEnd = vecStart - ( vecDirection * 4.0f );
			// cast a ray from our start pos to the end pos
			TraceLine ( vecStart, vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, pExitTrace );

			// check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if ( pExitTrace->startSolid && pExitTrace->surface.flags & SURF_HITBOX ) {
				TraceLine ( vecStart, vecPosition, MASK_SHOT_HULL, pExitTrace->entity, pExitTrace );

				if ( pExitTrace->did_hit ( ) && !pExitTrace->startSolid ) {
					vecStart = pExitTrace->end;
					return true;
				}

				continue;
			}

			if ( pExitTrace->did_hit ( ) && !pExitTrace->startSolid ) {
				if ( IsBreakableEntity ( pEnterTrace->entity ) && IsBreakableEntity ( pExitTrace->entity ) )
					return true;

				if ( ( !( pExitTrace->surface.flags & SURF_NODRAW ) || ( pEnterTrace->surface.flags & SURF_NODRAW && pExitTrace->surface.flags & SURF_NODRAW ) ) && pExitTrace->plane.normal.dot_product ( vecDirection ) <= 1.0f ) {
					const float flMultiplier = pExitTrace->flFraction * 4.0f;
					vecStart -= vecDirection * flMultiplier;
					return true;
				}

				continue;
			}

			if ( !pExitTrace->did_hit ( ) || pExitTrace->startSolid ) {
				if ( pEnterTrace->entity != nullptr && pEnterTrace->entity->index ( ) != 0 && IsBreakableEntity ( pEnterTrace->entity ) ) {
					// did hit breakable non world entity
					pExitTrace = pEnterTrace;
					pExitTrace->end = vecStart + vecDirection;
					return true;
				}

				continue;
			}
		}
	}

	return false;
}

bool autowall::HandleBulletPenetration ( player_t * pLocal, surfacedata_t * pEnterSurfaceData, weapon_info_t * pWeaponData, FireBulletData_t & data ) {
	static convar * ff_damage_reduction_bullets = interfaces::console->get_convar ( ( "ff_damage_reduction_bullets" ) );
	static convar * ff_damage_bullet_penetration = interfaces::console->get_convar ( ( "ff_damage_bullet_penetration" ) );

	const float flReductionDamage = ff_damage_reduction_bullets->get_float ( );
	const float flPenetrateDamage = ff_damage_bullet_penetration->get_float ( );

	const MaterialHandle_t hEnterMaterial = pEnterSurfaceData->game.material;
	const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

	trace_t exitTrace = { };
	if ( ( !data.iPenetrateCount &&
		!( data.enterTrace.surface.flags >> 7 & SURF_LIGHT ) && !( ( data.enterTrace.contents >> 3 ) & CONTENTS_SOLID ) &&
		hEnterMaterial != CHAR_TEX_GRATE && hEnterMaterial != CHAR_TEX_GLASS ) ||
		pWeaponData->flPenetration <= 0.0f ||
		( !TraceToExit ( &data.enterTrace, &exitTrace, data.enterTrace.end, data.vecDir ) && !( interfaces::trace_ray->get_point_contents ( data.enterTrace.end, MASK_SHOT_HULL, nullptr ) & MASK_SHOT_HULL ) ) )
		return false;

	const surfacedata_t * pExitSurfaceData = interfaces::physics->get_surface_data ( exitTrace.surface.surface_props );
	const MaterialHandle_t hExitMaterial = pExitSurfaceData->game.material;
	const float flExitPenetrationModifier = pExitSurfaceData->game.flPenetrationModifier;

	float flDamageLostModifier = 0.16f;
	float flPenetrationModifier = 0.0f;

	if ( hEnterMaterial == CHAR_TEX_GRATE || hEnterMaterial == CHAR_TEX_GLASS ) {
		flDamageLostModifier = 0.05f;
		flPenetrationModifier = 3.0f;
	}
	else if ( ( data.enterTrace.contents >> 3 ) & CONTENTS_SOLID || ( data.enterTrace.surface.flags >> 7 ) & SURF_LIGHT ) {
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = 1.0f;
	}
	else if ( hEnterMaterial == CHAR_TEX_FLESH && ( pLocal->team ( ) == data.enterTrace.entity->team ( ) && flReductionDamage == 0.0f ) ) {
		if ( flPenetrateDamage == 0.0f )
			return false;

		// shoot through teammates
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = flPenetrateDamage;
	}
	else {
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = ( flEnterPenetrationModifier + flExitPenetrationModifier ) * 0.5f;
	}

	if ( hEnterMaterial == hExitMaterial ) {
		if ( hExitMaterial == CHAR_TEX_CARDBOARD || hExitMaterial == CHAR_TEX_WOOD )
			flPenetrationModifier = 3.0f;
		else if ( hExitMaterial == CHAR_TEX_PLASTIC )
			flPenetrationModifier = 2.0f;
	}

	const float flTraceDistance = ( exitTrace.end - data.enterTrace.end ).length_sqr ( );

	// penetration modifier
	const float flModifier = std::max<float> ( 0.0f, 1.0f / flPenetrationModifier );

	// this calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
	const float flLostDamage = ( data.flCurrentDamage * flDamageLostModifier + std::max<float> ( 0.0f, 3.75f / pWeaponData->flPenetration ) * ( flModifier * 3.0f ) ) + ( ( flModifier * flTraceDistance ) / 24.0f );

	//did we loose too much damage?
	if ( flLostDamage > data.flCurrentDamage )
		return false;

	// we can't use any of the damage that we've lost
	if ( flLostDamage > 0.0f )
		data.flCurrentDamage -= flLostDamage;

	// do we still have enough damage to deal?
	if ( data.flCurrentDamage < 1.0f )
		return false;

	data.vecPosition = exitTrace.end;
	--data.iPenetrateCount;
	return true;
}

bool autowall::SimulateFireBullet ( player_t * pLocal, weapon_t * pWeapon, FireBulletData_t & data ) {
	weapon_info_t * pWeaponData = pWeapon->get_weapon_data ( );

	if ( pWeaponData == nullptr )
		return false;

	float flMaxRange = pWeaponData->flRange;
	float flTraceLenght = 0.0f;

	// the total number of surfaces any bullet can penetrate in a single flight is capped at 4
	data.enterTrace = { };
	data.iPenetrateCount = 4;
	data.flCurrentDamage = ( float ) pWeaponData->iDamage;

	trace_t trace = { };
	trace_filter_skip_one_entity filter ( pLocal );

	while ( data.iPenetrateCount > 0 && data.flCurrentDamage >= 1.0f ) {
		// max bullet range
		flMaxRange -= flTraceLenght;

		// end position of bullet
		const vec3_t vecEnd = data.vecPosition + data.vecDir * flMaxRange;
		TraceLine ( data.vecPosition, vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, pLocal, &data.enterTrace );

		// check for player hitboxes extending outside their collision bounds
		ClipTraceToPlayers ( data.vecPosition, vecEnd + data.vecDir * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &data.filter, &data.enterTrace );

		surfacedata_t * pEnterSurfaceData = interfaces::physics->get_surface_data ( data.enterTrace.surface.surface_props );
		float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

		// we didn't hit anything, stop tracing shoot
		if ( data.enterTrace.flFraction == 1.0f )
			break;

		// calculate the damage based on the distance the bullet travelled
		flTraceLenght += data.enterTrace.flFraction * flMaxRange;
		data.flCurrentDamage *= std::powf ( pWeaponData->flRangeModifier, flTraceLenght / MAX_DAMAGE );

		// check is actually can shoot through
		if ( flTraceLenght > 3000.0f || flEnterPenetrationModifier < 0.1f )
			break;

		bool did_hit_specific_target = false;
		if ( data.target && data.enterTrace.entity ) {
			did_hit_specific_target = data.enterTrace.entity == data.target;
		}
		else {
			did_hit_specific_target = data.enterTrace.entity ? data.enterTrace.entity->is_enemy ( ) : false;
		}



		// check is can do damage
		if ( data.enterTrace.hitGroup > hitgroup_generic && data.enterTrace.hitGroup <= hitgroup_rightleg && did_hit_specific_target ) {
			// we got target - scale damage
			ScaleDamage ( data.enterTrace.hitGroup, data.enterTrace.entity, pWeaponData->flArmorRatio, data.flCurrentDamage );
			return true;
		}
		data.penetrationTry++;
		// calling handlebulletpenetration here reduces our penetration �ounter, and if it returns true, we can't shoot through it
		if ( !HandleBulletPenetration ( pLocal, pEnterSurfaceData, pWeaponData, data ) )
			break;
	}

	return false;
}
void autowall::TraceLine ( const vec3_t & vecAbsStart, const vec3_t & vecAbsEnd, unsigned int fMask, player_t * pSkip, trace_t * pTrace ) {
	ray_t ray = { };
	ray.initialize ( vecAbsStart, vecAbsEnd );
	trace_filter_skip_one_entity filter ( pSkip );
	interfaces::trace_ray->trace_ray ( ray, fMask, &filter, pTrace ); /*crash */
}
float autowall::get_thickness ( vec3_t & start, vec3_t & end ) {
	vec3_t dir = end - start;
	vec3_t step = start;
	dir /= dir.length ( );
	trace_filter_skip_one_entity filter ( nullptr );

	trace_t trace;

	float thickness = 0;
	while ( true ) {
		ray_t ceau;
		ceau.initialize ( step, end );
		interfaces::trace_ray->trace_ray ( ceau, MASK_SOLID, &filter, &trace );

		if ( !trace.did_hit ( ) )
			break;

		const vec3_t lastStep = trace.end;
		step = trace.end;

		if ( ( end - start ).length ( ) <= ( step - start ).length ( ) )
			break;

		if ( !autowall::trace_to_exit_short ( step, dir, 5, 90 ) )
			return FLT_MAX;

		thickness += ( step - lastStep ).length ( );
	}
	return thickness;
}
bool autowall::trace_to_exit_short ( vec3_t & point, vec3_t & dir, const float step_size, float max_distance ) {
	float flDistance = 0;

	while ( flDistance <= max_distance ) {
		flDistance += step_size;

		point += dir * flDistance;
		int point_contents = interfaces::trace_ray->get_point_contents ( point, MASK_SHOT_HULL );
		if ( !( point_contents & MASK_SHOT_HULL ) ) {
			// found first free point
			return true;
		}
	}

	return false;
}
