#include "../features.hpp"

/*
using namespace player_manager;
std::deque<target> ragebot::targets;

bool ragebot::m_shoot_next_tick;

bool ragebot::is_point_is_safe( target & entity, vec3_t from, vec3_t point, int hitbox, matrix_t new_matrix [ MAXSTUDIOBONES ] ) {

	auto _hitbox = entity.hitbox_set->hitbox( hitbox );

	const auto mod = _hitbox->radius != -1.f?_hitbox->radius:0.f;
	vec3_t max = math::vector_transform ( _hitbox->maxs, new_matrix [ _hitbox->bone ] );
	vec3_t min = math::vector_transform ( _hitbox->mins, new_matrix [ _hitbox->bone ] );
	if ( auto intersection = get_intersect_point( from, point, min, max, _hitbox->radius ); intersection )
		return true;

	return false;
}

void ragebot::multi_point( target & entity, ragebot::points& points, const int& hit_box, player_manager::lagcomp_t& record ) {


	
	studio_box_t* hitbox = entity.hitbox_set->hitbox ( hit_box );


	
	const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
	 auto maxs = math::vector_transform ( hitbox->maxs + mod, record.bone[ hitbox->bone ] );
	 auto  mins = math::vector_transform ( hitbox->mins - mod, record.bone [ hitbox->bone ] );

	auto center_vec = (mins + maxs ) * 0.5f;

	ragebot::point center;
	center.is_safe = true;
	center.point = center_vec;
	points.center = center_vec;
	vec3_t CurrentAngles = math::calc_angle(center.point, engine_prediction::unpredicted_eye);

	vec3_t Forward = math::angle_vector( CurrentAngles );

	vec3_t Right = Forward.cross( vec3_t( 0, 0, 0.95f ) );
	vec3_t Left = vec3_t( -Right.x, -Right.y, Right.z );

	vec3_t Top = vec3_t( 0, 0, 0.99f );
	vec3_t Bot = vec3_t( 0, 0, -0.95f );
	

	if ( hit_box == hitbox_head ) {
	
		for ( auto i = 0; i < 6; ++i )
		{
			points.points.push_back( center );
		}
		vec3_t middle = ( Top - Bot );
#define scale(a, b)  hitbox->radius * (a / b )
		points.points [ 1 ].point += vec3_t( Right.x * scale ( variables::ragebot::head_scale, 100.f ), Right.y * scale ( variables::ragebot::head_scale, 100.f ), Top.z * hitbox->radius * 0.95f );
		points.points [ 2 ].point += vec3_t( Left.x * scale ( variables::ragebot::head_scale, 100.f ), Left.y * scale ( variables::ragebot::head_scale, 100.f ), Top.z * hitbox->radius * 0.95f );
		points.points [ 3 ].point += vec3_t( Right.x * scale ( variables::ragebot::head_scale, 100.f ), Right.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z );
		points.points [ 4 ].point += vec3_t( Left.x * scale ( variables::ragebot::head_scale, 100.f ), Left.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z  );
		points.points [ 5 ].point = vec3_t ( center_vec.x, center_vec.y, center_vec.z + Top.z * hitbox->radius * 0.99f );

		
	}
	else if ( hit_box >= hitbox_pelvis && hit_box <= hitbox_upper_chest )
	{
		float body_scale = ( variables::ragebot::double_tap && tickbase_system::m_shift_data.m_needs_recharge == 0 )?5.f:variables::ragebot::point_scale;
		for ( auto i = 0; i < 3; ++i )
		{
			points.points.emplace_back( center );
		}
		points.points [ 1 ].point += Right * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );
		points.points [ 2 ].point += Left * ( hitbox->radius * ( variables::ragebot::point_scale / 100.f ) );

	}
	else
		points.points.emplace_back( center );

	
	for ( size_t i = 0; i < points.points.size(); i++ )
	{
		if ( variables::ragebot::safe_point > 2 ) {
			points.points.at ( i ).is_safe = is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hit_box, csgo::left_player_bones [ entity.index ] ) && is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hit_box, csgo::right_player_bones [ entity.index ] );
		}
		else if ( variables::ragebot::safe_point == 1) {
			if ( hit_box == hitboxes::hitbox_head ) {
				auto left_head = entity.player->get_hitbox_position ( hitbox_head, csgo::left_player_bones [ entity.index ], entity.hitbox_set->hitbox ( hitbox_head ) );
				auto right_head = entity.player->get_hitbox_position ( hitbox_head, csgo::right_player_bones [ entity.index ], entity.hitbox_set->hitbox ( hitbox_head ) );
				auto left_head_w2s = vec3_t ( ), right_head_w2s = vec3_t ( );
				auto center = vec3_t ( csgo::screen_width / 2.f , csgo::screen_height / 2.f, 0.f );
				if ( interfaces::debug_overlay->world_to_screen ( left_head, left_head_w2s ) && interfaces::debug_overlay->world_to_screen ( right_head, right_head_w2s ) ) {
				
					auto dist_to_left = math::calc_distance ( center, left_head_w2s, true );
					auto dist_to_right = math::calc_distance ( center, right_head_w2s, true );
					
					
					points.points.at ( i ).is_safe =  is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hit_box,
						
						dist_to_left <= dist_to_right ? csgo::left_player_bones [ entity.index ] : csgo::right_player_bones [ entity.index ] );


				}
			}
			else {
				points.points.at ( i ).is_safe = is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hit_box, csgo::left_player_bones [ entity.index ] ) && is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hit_box, csgo::right_player_bones [ entity.index ] );
			}
		}
	}

}
/*
void ragebot::on_shot_safe ( target & entity, player_manager::lagcomp_t & record, ragebot::points & points ) {



	studio_box_t * hitbox = entity.hitbox_set->hitbox ( hitbox_head );

	const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;
	 auto maxs = math::vector_transform ( hitbox->maxs + mod, record.bone [ entity.index ][ hitbox->bone ] );
	 auto mins = math::vector_transform ( hitbox->mins - mod, record.bone [ entity.index ][ hitbox->bone ] );



	auto center_vec = ( mins + maxs ) * 0.5f;

	ragebot::point center;
	center.is_safe = false;
	center.point = center_vec;
	vec3_t CurrentAngles = math::calc_angle ( center.point, engine_prediction::unpredicted_eye );

	vec3_t Forward = math::angle_vector ( CurrentAngles );

	vec3_t Right = Forward.cross ( vec3_t ( 0, 0, 0.95f ) );
	vec3_t Left = vec3_t ( -Right.x, -Right.y, Right.z );

	vec3_t Top = vec3_t ( 0, 0, 0.95f );
	vec3_t Bot = vec3_t ( 0, 0, -0.95f );

	
		for ( auto i = 0; i < 6; ++i ) {
			points.points.push_back ( center );
		}
		vec3_t middle = ( Top - Bot );
#define scale(a, b)  hitbox->radius * (a / b )
		points.points [ 1 ].point += vec3_t ( Right.x * scale ( variables::ragebot::head_scale, 100.f ), Right.y * scale ( variables::ragebot::head_scale, 100.f ), Top.z * hitbox->radius * 0.95f );
		points.points [ 2 ].point += vec3_t ( Left.x * scale ( variables::ragebot::head_scale, 100.f ), Left.y * scale ( variables::ragebot::head_scale, 100.f ), Top.z * hitbox->radius * 0.95f );
		points.points [ 3 ].point += vec3_t ( Right.x * scale ( variables::ragebot::head_scale, 100.f ), Right.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z );
		points.points [ 4 ].point += vec3_t ( Left.x * scale ( variables::ragebot::head_scale, 100.f ), Left.y * scale ( variables::ragebot::head_scale, 100.f ), middle.z );
		points.points [ 5 ].point = vec3_t ( center_vec.x, center_vec.y, center_vec.z + Top.z * hitbox->radius * 0.95f );




	for ( size_t i = 0; i < points.points.size ( ); i++ ) {
		points.points.at ( i ).is_safe = is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hitbox_head, record.bone_left ) && is_point_is_safe ( entity, engine_prediction::unpredicted_eye, points.points.at ( i ).point, hitbox_head, record.bone_right);
		if (!points.points.at ( i ).is_safe )
			points.points.erase ( points.points.begin ( ) + i );
	}

}
*/



/*
void ragebot::createmove ( c_usercmd * cmd, bool & send_packet ) {


	static bool shot = false;
	static player_t * last_target = nullptr;

	if ( !variables::ragebot::enabled )
		return;

	if ( !tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready )
		m_shoot_next_tick = false;
	sort_targets ( );

	ragebot::target_data best_target; best_target.aimpos = vec3_t ( ); best_target.dmg = 0.f; best_target.entity = nullptr;
	best_target.backtrack = false;
	best_target.extrapolation = false;
	target best_target_ent;
	for ( auto ent : targets ) {

		auto target_data = get_target_data ( ent );

		if ( !target_data.aimpos.is_zero ( ) && best_target.dmg <= target_data.dmg ) {
			best_target.dmg = target_data.dmg;
			best_target.aimpos = target_data.aimpos;
			best_target.entity = ent.player;
			best_target.hitbox = target_data.hitbox;
			best_target.backtrack = target_data.backtrack;
			best_target.record = target_data.record;
			best_target.aimpos_center = target_data.aimpos_center;
			best_target_ent = ent;

		}
	}


	if ( !best_target.aimpos.is_zero ( ) && best_target.dmg != float {} ) {



		//	printf( "SET BT SIMTIME %f (%f) PLAYER SIMTIME  TICKS: %i\n", player_manager::best_tick_global [ best_target.entity->index( ) ].simtime, sim_time,  math::time_to_ticks(variables::cheat:interval_per_tick, best_target.entity->simulation_time( ) - player_manager::best_tick_global [ best_target.entity->index( ) ].simtime ) );

		//printf( "SIMTIME %f \n", player_manager::best_tick_global [ best_target.entity->index//( ) ].simtime );
		bool using_scout = csgo::local_player->active_weapon ( )->item_definition_index ( ) == weapon_ssg08;
		if ( variables::ragebot::autostop && csgo::local_player->flags ( ) & fl_onground ) {
			if ( using_scout ) {
				if ( best_target.dmg >= best_target.entity->health ( ) && csgo::local_player->can_shoot ( false ) ) {
					cmd->sidemove = 0;
					cmd->forwardmove = csgo::local_player->velocity ( ).Length2D ( ) > 20.f ? 450.f : 0.f;


					autostop::rotate_movement ( cmd, math::calc_angle ( vec3_t ( 0, 0, 0 ), csgo::local_player->velocity ( ) ).y + 180.f );
				}
			}
			else {
				cmd->sidemove = 0;
				cmd->forwardmove = csgo::local_player->velocity ( ).Length2D ( ) > 20.f ? 450.f : 0.f;

				autostop::rotate_movement ( cmd, math::calc_angle ( vec3_t ( 0, 0, 0 ), csgo::local_player->velocity ( ) ).y + 180.f );
			}
		}




		auto ang = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimpos );
		auto hc_ang = math::calc_angle ( engine_prediction::unpredicted_eye, best_target.aimpos_center );
		if (variables::ragebot::nospread )
		ang -= csgo::local_player->aim_punch_angle ( ) * 2.f;
		math::clamp ( ang );

		float sim_time = 0.f;

		

		best_target.simtime = best_target.backtrack || best_target.extrapolation ? best_target.record.simtime : best_target.entity->simulation_time ( );
		if ( !best_target.extrapolation )
			best_target.simtime += player_manager::get_lerp_time ( );
		else
			interfaces::console->console_printf ( "EXTRAPOLATING ... %b", best_target.extrapolation );

		
		if ( hit_chance ( best_target_ent, best_target.aimpos, hc_ang, variables::ragebot::hitchance, best_target ) ) {

			sleep_ticks++;
			if ( !variables::ragebot::silent_aim )
				interfaces::engine->set_view_angles ( ang );
			/*if ( !tickbase_system::m_shift_data.m_should_attempt_shift || ( ( csgo::m_goal_shift == 12 || tickbase_system::m_shift_data.m_should_disable ) && tickbase_system::m_shift_data.m_should_attempt_shift ) || ( csgo::m_goal_shift == 7 && tickbase_system::m_shift_data.m_should_attempt_shift && !( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) ) {
				{
					if (variables::ragebot::auto_fire && csgo::local_player->can_shoot(false))
					cmd->buttons |= (int)in_attack;
				else if (variables::ragebot::auto_fire && !csgo::local_player->can_shoot( false ))
					cmd->buttons &= ~(int)in_attack;

				}
			}*/

			/*if ( variables::ragebot::auto_fire && csgo::local_player->can_shoot ( variables::ragebot::double_tap ) )
				cmd->buttons |= ( int ) in_attack;
			else if ( variables::ragebot::auto_fire && !csgo::local_player->can_shoot ( variables::ragebot::double_tap ) )
				cmd->buttons &= ~( int ) in_attack;

			if ( cmd->buttons & ( int ) in_attack ) {

				cmd->tick_count = math::time_to_ticks ( best_target.simtime );
				//interfaces::console->console_printf( "SIMTIME BETWEEN SHOTS %f \n", fabs( ragebot::get_last_target.simtime - best_target.simtime ) );
				cmd->viewangles = ang;
				send_packet = true;
				ragebot::get_last_target = best_target;
				ragebot::last_target_index = best_target.entity->index ( );
				if ( m_shoot_next_tick )
					m_shoot_next_tick = false;

			
				if ( best_target.extrapolation )
					interfaces::console->console_printf ( "EXTRAPOLATED %b \n", best_target.extrapolation );
				csgo::choke_next_tick = true;
				if ( !m_shoot_next_tick && csgo::m_goal_shift == variables::antiaim::db_tap_ticks && tickbase_system::m_shift_data.m_should_attempt_shift && !( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) {
					m_shoot_next_tick = true;
				}
				if ( autostop::m_autostop_data.state == 1 )
					autostop::m_autostop_data.state = 0;

			}
		}
		
		//if ( best_target.backtrack ) player_manager::restore_player( best_target.entity );
	}
}*/