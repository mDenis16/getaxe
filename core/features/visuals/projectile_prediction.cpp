#include "../features.hpp"
#include "../../../dependencies/imgui/imgui.h"

#include "../../../dependencies/imgui/imgui_internal.h"
#include "../../render/gui/gui.h"

static const float SmokeGrenadeRadius_InfernoAffectingZ = 120.0f;
// When interacting with fire on the same plane we don't want alpha depth-fighting
// in the most common case, so leave a grace margin between the smoke particles
// and the fire particles.
static const float SmokeGrenadeRadius_InfernoAffectingXY_topedge = 100.0f;
static const float SmokeGrenadeRadius_InfernoAffectingXY_equator = 150.0f;
static const float SmokeGrenadeRadius_InfernoAffectingXY_bottomedge = 128.0f;

// Fire burning things and smoke constants
static const float InfernoFire_HalfWidth = 30.0f;
static const float InfernoFire_FullHeight = 80.0f;

#define INFERNO_MASK_TO_GROUND ((MASK_SOLID_BRUSHONLY) & (~CONTENTS_GRATE))
#define INFERNO_MASK_LOS_CHECK ( INFERNO_MASK_TO_GROUND | CONTENTS_MONSTER )
#define INFERNO_MASK_DAMAGE INFERNO_MASK_LOS_CHECK

#define DAMAGE_YES  2

namespace visuals {

	uint32_t get_filter_simple_vtable ( ) {
		static uint8_t* address = NULL;
		if ( !address )
			address = utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52" );


		return *reinterpret_cast< uint32_t * >( address + 0x3d );

	}

   void UTIL_TraceLine (  vec3_t & vecAbsStart,  vec3_t & vecAbsEnd, unsigned int mask,
		 void * ignore, int collisionGroup, trace_t * ptr ) {
		uint32_t filter [ 4 ] = { get_filter_simple_vtable ( ), uint32_t ( ignore ), 0, 0 };

		ray_t t; t.initialize ( vecAbsStart, vecAbsEnd, vec3_t ( -1.f, -1.f, -1.f ), vec3_t ( 1.f, 1.f, 1.f ) );

		interfaces::trace_ray->trace_ray ( t, MASK_SOLID, ( i_trace_filter * ) &filter, ptr );
	}

    entity_t* get_closest_entity ( entity_t* comparator, vec3_t position, int class_filter, int size ) {

		entity_t * ent = nullptr;
	
		for ( auto & entity : visuals::handler->entity_list ) {
			if ( entity->entity != comparator && entity->entity->client_class()->class_id == class_filter ) {
				int dist = math::calc_distance ( position, entity->origin, false );
				int dist2 = position.distance_to ( entity->origin );

				if ( dist < size ) {
					ent = entity->entity;
					size = dist;
				}
			}
		}

		return ent;
    }

	bool BShouldExtinguishSmokeGrenadeBounce ( entity_t * entity, vec3_t & posDropSmoke )  {

		/*experimental shit*/

		vec3_t local_pop_smoke = posDropSmoke;

		trace_t trSmokeTrace;
		vec3_t p = local_pop_smoke - vec3_t ( 0, 0, 166 );
		UTIL_TraceLine ( local_pop_smoke, p, ( MASK_PLAYERSOLID & ~CONTENTS_PLAYERCLIP ),
			entity, 0, &trSmokeTrace );

		if ( !trSmokeTrace.startSolid ) {
			if ( trSmokeTrace.flFraction >= 1.0f )
				return false;	// this smoke cannot drop enough to cause extinguish

			if ( trSmokeTrace.flFraction > 0.001f )
				local_pop_smoke = trSmokeTrace.end;
		}

		const float radius = 2.0f * InfernoFire_HalfWidth;

		int		m_fireCount = *reinterpret_cast< int * >		( ( DWORD ) entity + 0x13A8		/*0x13A8*/ );
		bool * m_bFireIsBurning = reinterpret_cast< bool * >	( ( DWORD ) entity + 0xE94 /*0xE94*/ );


		for ( int i = 0; i < entity->fireCount(); i++ ) {
			
			if ( !m_bFireIsBurning [ i ] )
				continue;


			int * m_fireXDelta = reinterpret_cast< int * >		( ( DWORD ) entity + 0x9E4		/*0x9E4*/ );
			int * m_fireYDelta = reinterpret_cast< int * >		( ( DWORD ) entity + 0xB74		/*0xB74*/ );
			int * m_fireZDelta = reinterpret_cast< int * >		( ( DWORD ) entity + 0xD04		/*0xD04*/ );


			vec3_t center = entity->origin() + vec3_t ( m_fireXDelta [ i ], m_fireYDelta [ i ], m_fireZDelta [ i ] );

			if ( ( local_pop_smoke - center ).length() <  ( radius ) ) {
				// doublecheck los if required
				trace_t tr;
				 vec3_t fireHeight ( 0, 0, InfernoFire_HalfWidth );
				 vec3_t res = center + fireHeight;

				UTIL_TraceLine ( res, local_pop_smoke, INFERNO_MASK_DAMAGE, entity, 0, &tr );
				if ( tr.flFraction < 1.0f )
					UTIL_TraceLine ( center, local_pop_smoke, INFERNO_MASK_DAMAGE, entity, 0, &tr );
				if ( tr.flFraction == 1.0f ) {
					/*if ( InfernoDebug.GetBool ( ) ) {
						NDebugOverlay::Line ( fire->m_center, posDropSmoke, 255, 0, 255, true, 50.2f );
					}*/
					std::cout << "cica a mers " << std::endl;
 
					return true;
				}
				else {
				/*	if ( InfernoDebug.GetBool ( ) ) {
						NDebugOverlay::Line ( fire->m_center, posDropSmoke, 255, 0, 0, true, 50.2f );
					}*/
				}
			}
		}

		return false;
	}

	bool CSmokeGrenadeProjectile_OnBounced ( grenade_t * grenade, vec3_t bounch_pos ) {

		entity_t * closest_inferno = get_closest_entity ( grenade, bounch_pos, class_ids::CInferno, 512 );
		if ( !closest_inferno )
			return false;

		if ( BShouldExtinguishSmokeGrenadeBounce ( closest_inferno, bounch_pos ) ) {
			return true;
		}

		return false;
	}
	void grenade_prediction::Simulate ( entity_t * last_hit_player, grenade_t* grenade, std::vector<vec3_t>& path,  float elasticty, int type, vec3_t * vecSrc ) {
	
		float interval = interfaces::globals->interval_per_tick;
		
		// Log positions 20 times per sec
		int logstep = static_cast< int >( 0.05f / interval );
		int logtimer = 0;
	
		vec3_t vecThrow = grenade->m_vInitialVelocity ( );

		for ( int i = 0; i < 500 - 1; ++i ) {
			if ( !logtimer )
				path.push_back ( *vecSrc );

			int s = Step ( last_hit_player, grenade, *vecSrc, vecThrow, elasticty, i, interval, type );
			if ( ( s & 1 ) ) 
				break;
			if ( ( s & 2 ) || logtimer >= logstep )
				logtimer = 0;
			else
				++logtimer;

		//	if ( vecThrow.Length2D() < 0.1f ) break;
		}
		path.push_back ( *vecSrc );
		
	}

	int grenade_prediction::Step ( entity_t* last_hit_player, grenade_t * grenade, vec3_t & vecSrc, vec3_t & vecThrow, float elasticity, int tick, float interval, int type ) {

		vec3_t move;
		AddGravityMove ( grenade, move, vecThrow, interval, false );

		trace_t tr;
		PushEntity ( grenade, vecSrc, move, tr );


		int result = 0;

	

		if ( CheckDetonate ( grenade, vecThrow, tr, tick, interval, type ) )
			result |= 1;

		

		if ( tr.flFraction != 1.0f ) {
			result |= 2;
			ResolveFlyCollisionCustom ( last_hit_player, grenade, tr, elasticity, vecThrow, interval );
			if ( type == CSmokeGrenadeProjectile && CSmokeGrenadeProjectile_OnBounced ( grenade, tr.end ) )
				result |= 1; /* smoke extingushed by inferno */
		}
	

		vecSrc = tr.end;

	

		return result;
	}


	bool grenade_prediction::CheckDetonate ( grenade_t* grenade, const vec3_t & vecThrow, const trace_t & tr, int tick, float interval, int type ) {

		float ping = 0.f;

		ping += interfaces::engine->get_net_channel_info ( )->get_latency ( 0 );
		ping += interfaces::engine->get_net_channel_info ( )->get_latency ( 1 );

		switch ( type ) {
		case CSmokeGrenadeProjectile:
		case CDecoyProjectile:
			if ( sqrtf ( vecThrow.x * vecThrow.x + vecThrow.y * vecThrow.y ) < 0.1f ) {
				int det_tick_mod = static_cast< int >( 0.2f / interval );
				return !( tick % det_tick_mod );
			}
			return false;
		case CMolotovProjectile:
		case CIncendiaryGrenade:
			if ( tr.entity && tr.entity->index ( ) == 0 && tr.flFraction != 1.0f && tr.plane.normal.z > 0.7f )
				return true;
			 else if ( static_cast< float >( tick ) * interval > 2.0f)
				return true;
			 return false;
			//	return true;static_cast< float >( tick ) * interval > 2.0;
		case CBaseCSGrenadeProjectile:
			return static_cast< float >( tick ) * interval > 1.6f + ping;// && !( tick % static_cast< int >( 0.2f / interval ) );
		default:
			return false;
		}
	}

	void grenade_prediction::TraceHull ( grenade_t * grenade, vec3_t & src, vec3_t & end, trace_t & tr ) {

		uint32_t filter [ 4 ] = { get_filter_simple_vtable ( ), uint32_t ( grenade ), 0, 0 };

		ray_t t; t.initialize ( src, end, vec3_t( -1.f, -1.f, -1.f ), vec3_t ( 1.f, 1.f, 1.f ) );

		interfaces::trace_ray->trace_ray ( t, MASK_SOLID, ( i_trace_filter * ) &filter, &tr );
		
	}

	void grenade_prediction::AddGravityMove ( grenade_t * grenade, vec3_t & move, vec3_t & vel, float frametime, bool onground ) {
		vec3_t basevel ( 0.0f, 0.0f, 0.0f );

		move.x = ( vel.x + basevel.x ) * frametime;
		move.y = ( vel.y + basevel.y ) * frametime;

		if ( onground ) {
			move.z = ( vel.z + basevel.z ) * frametime;
		}
		else {
			// Game calls GetActualGravity( this );
			float gravity = 800.0f * 0.4f;

			float newZ = vel.z - ( gravity * frametime );
			move.z = ( ( vel.z + newZ ) / 2.0f + basevel.z ) * frametime;

			vel.z = newZ;
		}
	}

	void grenade_prediction::PushEntity ( grenade_t * grenade, vec3_t & src, const vec3_t & move, trace_t & tr ) {

		vec3_t vecAbsEnd = src;
		vecAbsEnd += move;

		// Trace through world
		TraceHull ( grenade, src, vecAbsEnd, tr );
	}

	void grenade_prediction::ResolveFlyCollisionCustom ( entity_t* last_hit_player, grenade_t * grenade, trace_t & tr, float elasticity, vec3_t & vecVelocity, float interval ) {

		/*std::cout << " ResolveFlyCollisionCustom " << std::endl;

		const float kSleepVelocity = 20.0f;
		const float kSleepVelocitySquared = kSleepVelocity * kSleepVelocity;

		/*if ( breakthrough ) {


			if ( pEntity->m_iHealth () <= 0 ) {
				// slow our flight a little bit

				vecVelocity *= 0.4;
				return;
			}
		}*/

		/*surfacedata_t * pEnterSurfaceData = interfaces::physics->get_surface_data ( tr.surface.surface_props );
		float flSurfaceElasticity = 1.00f;// pEnterSurfaceData->physics.elasticity;

		//if ( pEntity && pEntity->is_player ( ) ) {
		//	flSurfaceElasticity = 0.3f;
		//}


		float flGrenadeElasticity = elasticity; // GetGrenadeElasticity()
		float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
		flTotalElasticity = std::clamp ( flTotalElasticity, 0.0f, 0.9f );

		vec3_t vecAbsVelocity;
		PhysicsClipVelocity ( grenade, vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f );
		vecAbsVelocity *= flTotalElasticity;


		float flSpeedSqr = vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y + vecVelocity.z * vecVelocity.z;
#if 1 //i found two different methods of doing this, they are very similar and both give the exact same result 
		if ( tr.plane.normal.z > 0.7f ) // Floor
		{
			std::cout << " ResolveFlyCollisionCustom HIT FLOOR " << std::endl;
			if ( flSpeedSqr > 96000 ) {
				float alongDist = vecAbsVelocity.normalized().dot ( tr.plane.normal );// ( vecAbsVelocity.Normalized ( ), trace.plane.normal );
				if ( alongDist > 0.5f ) {
					float flBouncePadding = ( 1.0f - alongDist ) + 0.5f;
					vecAbsVelocity *= flBouncePadding;
					std::cout << " ResolveFlyCollisionCustom  CORRECTED FLOOR BOUNDING " << std::endl;
				}
			}

			// Verify that we have an entity.
			vecVelocity = vecAbsVelocity;
			
			vecAbsVelocity *= ( ( 1.0f - tr.flFraction ) * interval );
			PushEntity ( grenade, tr.end, vecAbsVelocity, tr );
			std::cout << " ResolveFlyCollisionCustom PUSH ENTITY IN AIR AGAIN" << std::endl;
			
		}
		else {
			
			vecVelocity = vecAbsVelocity;
			
		}

#else
		// Stop completely once we move too slow
		//float flSpeedSqr = vecAbsVelocity.LengthSqr();
		static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
		if ( flSpeedSqr < flMinSpeedSqr ) {
			vecAbsVelocity.x = 0;
			vecAbsVelocity.y = 0;
			vecAbsVelocity.z = 0;
		}

		// Stop if on ground
		if ( tr.plane.normal.z > 0.7f ) {
			vecVelocity = vecAbsVelocity;
			vecAbsVelocity *= ( ( 1.0f - tr.fraction ) * interval );
			PushEntity ( tr.endpos, vecAbsVelocity, tr );
		}
		else {
			vecVelocity = vecAbsVelocity;
		}
#endif*/

		entity_t * pEntity = tr.entity;
		bool IsWorld = tr.did_hit_world ( );
		bool bIsProjectile = false;
		bool bIsHostage = false;
		
	
		bool breakthrough = false;

		if ( pEntity ) {
			int class_id = pEntity->client_class ( )->class_id;
			if ( pEntity && class_id == CBreakableProp ) {
				breakthrough = true;
			}

			if ( pEntity && class_id == CBreakableSurface ) {
				breakthrough = true;
			}

			if ( pEntity && class_id == CPhysicsPropMultiplayer && pEntity->m_iHealth ( ) > 0 && pEntity->get_take_damage ( ) == DAMAGE_YES ) {
				breakthrough = true;
			}

			// this one is tricky because BounceTouch hits breakable propers before we hit this function and the damage is already applied there (CBaseGrenade::BounceTouch( CBaseEntity *pOther ))
			// by the time we hit this, the prop hasn't been removed yet, but it broke, is set to not take anymore damage and is marked for deletion - we have to cover this case here
			if ( pEntity && class_id == CDynamicProp && pEntity->m_iHealth ( ) > 0 &&  pEntity->get_take_damage ( ) == DAMAGE_YES /*|| ( pEntity->get_take_damage() == DAMAGE_NO && pEntity->IsEFlagSet ( EFL_KILLME ) ) )*/ /*idk?? */ ) {
				breakthrough = true;
			}
		}

		if ( pEntity && breakthrough ) {
			
			if ( pEntity->m_iHealth() <= 0 ) {
				// slow our flight a little bit
			
				vecVelocity *= 0.4;
				return;
			}
		}

		float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity

		//Don't bounce off of players with perfect elasticity
		if ( pEntity ) {
			int hit_classid = pEntity->client_class ( )->class_id;
			std::cout << "hit classid " << hit_classid << std::endl;
			if ( pEntity->client_class ( )->class_id == CCSPlayer ) {
				flSurfaceElasticity = 0.3f;
			}
		}

		if ( pEntity && pEntity->index() != 0 && last_hit_player == pEntity ) {
			bool bIsHostage = false;
			if ( pEntity->client_class ( )->class_id == CCSPlayer || bIsHostage || bIsProjectile ) {
				return;
			}
		}

		if ( pEntity ) {
			last_hit_player = pEntity;
		}


		const float kSleepVelocity = 20.0f;
		const float kSleepVelocitySquared = kSleepVelocity * kSleepVelocity;

		
		float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
		float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
		flTotalElasticity = std::clamp ( flTotalElasticity, 0.0f, 0.9f );

		// NOTE: A backoff of 2.0f is a reflection
		vec3_t vecAbsVelocity;
		PhysicsClipVelocity (grenade, vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f );
		vecAbsVelocity *= flTotalElasticity;

		
		float flSpeedSqr = vecAbsVelocity.length_sqr ( );
		
		bool bIsWeapon = false;

		if ( pEntity &&
			( ( tr.plane.normal.z > 0.7f ) || ( tr.plane.normal.z > 0.1f && flSpeedSqr < kSleepVelocitySquared ) ) &&
			( pEntity->IsStandable ( ) || bIsProjectile || bIsWeapon || IsWorld  )
			) {
			if ( flSpeedSqr > 96000 ) {
				float alongDist = vecAbsVelocity.normalized().dot ( tr.plane.normal );// ( vecAbsVelocity.Normalized ( ), trace.plane.normal );
				if ( alongDist > 0.5f ) {
					float flBouncePadding = ( 1.0f - alongDist ) + 0.5f;
					vecAbsVelocity *= flBouncePadding;
					std::cout << " ResolveFlyCollisionCustom  CORRECTED FLOOR BOUNDING " << std::endl;
				}
			}

			vecVelocity = vecAbsVelocity;

			if ( flSpeedSqr < kSleepVelocitySquared ) {
				vecAbsVelocity = vec3_t ( 0, 0, 0 );
				vecVelocity = vecAbsVelocity;
			}
			else { 
				//vecAbsVelocity *= ( ( 1.0f - tr.flFraction ) * interval ); 
				/*vec3_t vecBaseDir = grenade->velocity ( );
				if ( !vecBaseDir.is_zero ( ) ) {
					vecBaseDir.normalize_in_place ( );
					vec3_t vecDelta = vecBaseDir - vecAbsVelocity;
					float flScale = vecDelta.dot ( vecBaseDir );
					vecAbsVelocity += vecBaseDir * flScale;
				}

				PushEntity ( grenade, tr.end, vecAbsVelocity, tr );*/
				vecVelocity = vecAbsVelocity;

				PushEntity ( grenade, tr.end, vecAbsVelocity * ( ( 1.f - tr.flFraction ) * interval ), tr );
				
			}
		}
		else {
			vecVelocity = vecAbsVelocity;
			vecAbsVelocity *= ( ( 1.0f - tr.flFraction ) * interval ); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
			PushEntity ( grenade, tr.end, vecAbsVelocity, tr );
		}

		/*if ( tr.plane.normal.z > 0.7f ) {
			auto flSpeedSqr = vecAbsVelocity.length_sqr ( );
			if ( flSpeedSqr > 96000.f ) {
				auto l = vecAbsVelocity.normalized ( ).dot ( tr.plane.normal );
				if ( l > 0.5f )
					vecAbsVelocity *= 1.f - l + 0.5f;
			}

			if ( flSpeedSqr < 20.f * 20.f )
				vecVelocity = vec3_t ( 0, 0, 0 );
			else {
				vecVelocity = vecAbsVelocity;

				PushEntity (grenade, tr.end, vecAbsVelocity * ( ( 1.f - tr.flFraction ) * interfaces::globals->interval_per_tick ), tr );
			}
		}
		else {
			vecVelocity = vecAbsVelocity;

			PushEntity ( grenade, tr.end, vecAbsVelocity * ( ( 1.f - tr.flFraction ) * interfaces::globals->interval_per_tick ), tr );
		}*/


	}
		

	int grenade_prediction::PhysicsClipVelocity ( grenade_t * grenade, const vec3_t & in, const vec3_t & normal, vec3_t & out, float overbounce ) {
		static const float STOP_EPSILON = 0.1f;
		
		
		auto backoff = in.dot_product ( normal ) * overbounce;
		for ( auto i = 0; i < 3; ++i ) {
			auto change = normal [ i ] * backoff;
			out [ i ] = in [ i ] - change;
			if ( out [ i ] > -STOP_EPSILON && out [ i ] < STOP_EPSILON )
				out [ i ] = 0.f;
		}
		return 0;
	}
}