#include "../features.hpp"

vec3_t fake_lag::peek_position = vec3_t( );
void fake_lag::on_peek( c_usercmd* cmd, bool& send_packet ) {


	static bool Hitinit = false;
	bool SkipTick = false;
	static bool canHit = false;
	bool HitCheck = false;
	
	if ( config.antiaim_fakelag_flags[3] && !config.ragebot_double_tap)
	{
		vec3_t peek_position_temp = vec3_t();
		for ( auto ent : aimbot::targets )
		{


			vec3_t EnemyHead = { ent.player->origin( ).x, ent.player->origin( ).y, ( ent.player->get_hitbox_position( 0 ).z + 10.f ) };

			vec3_t Head = { local_player::m_data.pointer->origin( ).x, local_player::m_data.pointer->origin( ).y, ( local_player::m_data.pointer->get_hitbox_position( 0 ).z + 10.f ) };
			vec3_t HeadExtr = ( Head + ( local_player::m_data.pointer->velocity( ) * math::ticks_to_time( 14 ) ) );
			vec3_t OriginExtr = ( ( local_player::m_data.pointer->origin( ) + ( local_player::m_data.pointer->velocity( ) * math::ticks_to_time( 22 ) ) ) + vec3_t( 0, 0, 8 ) );
		
			if ( fabs( local_player::m_data.pointer->velocity( ).Length2D( ) ) > .1f && ( autowall::can_hit_float_point( HeadExtr, EnemyHead ) || autowall::can_hit_float_point( OriginExtr, EnemyHead ) ) )
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
			if ( (interfaces::engine->get_net_channel( )->choked_packets < 14))
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

	if ( !local_player::m_data.alive )
		return;



	int choke_target = 2;
	int player_state = 0;
	if ( local_player::m_data.pointer->is_moving ( ) )
		player_state = 1;
	else
		player_state = 0;

	if ( local_player::m_data.pointer->is_in_air ( ) )
		player_state = 2;

	if ( config.antiaim_fakelag_flags [ player_state ] )
		choke_target = config.antiaim_fakelag > 0 ? math::random_int ( 10, 14 ) : 0;

	
	send_packet = interfaces::engine->get_net_channel ( )->choked_packets >= choke_target;


	on_peek ( cmd, send_packet );
	
}
