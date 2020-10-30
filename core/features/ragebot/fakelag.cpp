#include "../features.hpp"

vec3_t fake_lag::peek_position = vec3_t( );
void fake_lag::on_peek( c_usercmd* cmd, bool& send_packet ) {

	static bool Hitinit = false;
	bool SkipTick = false;
	static bool canHit = false;
	bool HitCheck = false;
	
	if ( variables::antiaim::on_peek && !variables::ragebot::double_tap )
	{
		vec3_t peek_position_temp = vec3_t();
		for ( auto ent : ragebot::targets )
		{


			vec3_t EnemyHead = { ent->origin( ).x, ent->origin( ).y, ( ent->get_hitbox_position( 0 ).z + 10.f ) };

			vec3_t Head = { csgo::local_player->origin( ).x, csgo::local_player->origin( ).y, ( csgo::local_player->get_hitbox_position( 0 ).z + 10.f ) };
			vec3_t HeadExtr = ( Head + ( csgo::local_player->velocity( ) * math::ticks_to_time( 22 ) ) );
			vec3_t OriginExtr = ( ( csgo::local_player->origin( ) + ( csgo::local_player->velocity( ) * math::ticks_to_time( 22 ) ) ) + vec3_t( 0, 0, 8 ) );
		
			if ( fabs( csgo::local_player->velocity( ).Length2D( ) ) > .1f && ( autowall::can_hit_float_point( HeadExtr, EnemyHead ) || autowall::can_hit_float_point( OriginExtr, EnemyHead ) ) )
			{
				if ( !Hitinit )
				{
					canHit = true;
					Hitinit = true;
					peek_position_temp = OriginExtr;
				}
				HitCheck = true;
			}
		}

		if ( !HitCheck )
			Hitinit = false;

		if ( canHit )
		{
			send_packet = true;
			SkipTick = true;
			csgo::lagPeek = true;
			fake_lag::peek_position = peek_position_temp;
			canHit = false;
		}

		if ( csgo::lagPeek && !SkipTick )
		{
			if ( (interfaces::engine->get_net_channel( )->choked_packets < 22))
			{
				//tickbase_system::m_shift_data.m_needs_recharge = csgo::m_goal_shift;
				send_packet = false;
				
		//		autostop::slow_walk( csgo::cmd, true );
			}
			else {
				csgo::lagPeek = false;
				fake_lag::peek_position = vec3_t( );
			

			}
		}
	}
}

void fake_lag::create_move( c_usercmd* cmd, bool& send_packet ) {
	on_peek( cmd, send_packet );
	if ( tickbase_system::m_shift_data.m_should_attempt_shift && ( ( !tickbase_system::m_shift_data.m_should_be_ready && tickbase_system::m_shift_data.m_prepare_recharge ) || tickbase_system::m_shift_data.m_needs_recharge || tickbase_system::m_shift_data.m_should_be_ready ) ) {
		//	send_packet = true;
	}

	//if ( !variables::ragebot::double_tap && ragebot::targets.size() > 0 && ragebot::targets.at( 0 ) && ragebot::targets.at( 0 )->active_weapon( ) && ragebot::targets.at( 0 )->active_weapon( )->item_definition_index( ) == weapon_ssg08 )
	//	send_packet = interfaces::engine->get_net_channel( )->choked_packets >= 22;
	
	
	//if ( csgo::local_player->get_anim_state( ) )
//	interfaces::console->console_printf( "LAST TIME MOVING %i \n", math::time_to_ticks( csgo::local_player->get_anim_state( )->m_flTimeSinceStartedMoving) );
	if (variables::antiaim::chokeAfterShoot && csgo::local_player->active_weapon( ) ) {
		int ticks_after_shot = interfaces::globals->tick_count - math::time_to_ticks( csgo::local_player->active_weapon( )->m_fLastShotTime( ));
		if ( ticks_after_shot > 0 && ticks_after_shot <= 22  )
		{
			send_packet = false;
			//interfaces::console->console_printf( "CHOKE AFTER SHOOT TICK %i \n",  ticks_after_shot );
		}
	}
		
}