#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../../core/features/nade prediction/nade_prediction.hpp"
#include "../menu/menu.hpp"

hooks::frame_stage::fn frame_stage_original = nullptr;
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::post_screen_space_fx::fn post_screen_space_original = nullptr;
hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::crc_server_check::fn crc_server_check_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::setup_bones::fn setup_bones_original = nullptr;
hooks::do_extra_bone_processing::fn do_extra_bone_processing_original = nullptr;
hooks::standard_blending_rules::fn standard_blending_rules_original = nullptr;
hooks::update_client_side_animation::fn update_client_side_animation_original = nullptr;
hooks::render_smoke_overlay::fn render_smoke_overlay_original = nullptr;
hooks::trace_ray::fn trace_ray_original = nullptr;
hooks::fire_event::fn fire_event_original = nullptr;
hooks::eye_angles::fn eye_angles_original = nullptr;
hooks::write_user_cmd::fn write_user_cmd_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::cl_move::fn cl_move_original = nullptr;
hooks::run_commmand::fn run_commmand_original = nullptr;
void* do_procedural_foot_plant_original = nullptr;

static constexpr auto BONE_USED_BY_SERVER = BONE_USED_BY_HITBOX | BONE_USED_BY_VERTEX_LOD0 | BONE_USED_BY_VERTEX_LOD1 | BONE_USED_BY_VERTEX_LOD2
| BONE_USED_BY_VERTEX_LOD3 | BONE_USED_BY_VERTEX_LOD4 | BONE_USED_BY_VERTEX_LOD5 | BONE_USED_BY_VERTEX_LOD6 | BONE_USED_BY_VERTEX_LOD7;

bool hooks::initialize( ) {


	static const auto c_cs_player_table = reinterpret_cast< uint32_t >( utilities::pattern_scan( "client.dll",
		"55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x47;

	void* pt = reinterpret_cast< void* >( c_cs_player_table );
	static const auto _sbf = utilities::pattern_scan( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) ) + 0x4E;
	void* sbf = reinterpret_cast< void* > ( _sbf );
	auto frame_stage_target = reinterpret_cast< void* >( get_virtual( interfaces::client, 37 ) );
	auto create_move_target = reinterpret_cast< void* >( get_virtual( interfaces::clientmode, 24 ) );
	auto paint_traverse_target = reinterpret_cast< void* >( get_virtual( interfaces::panel, 41 ) );
	auto post_screen_space_fx = reinterpret_cast< void* >( get_virtual( interfaces::clientmode, 44 ) );
	auto draw_model_execute_target = reinterpret_cast< void* >( get_virtual( interfaces::model_render, 21 ) );
	auto scene_end_target = reinterpret_cast< void* >( get_virtual( interfaces::render_view, 9 ) );
	auto setup_bones_target = reinterpret_cast< void* >( get_virtual( sbf, 13 ) );
	auto do_procedural_foot_plant_target = reinterpret_cast< void* >( utilities::pattern_scan( "client.dll", "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B 56 60" ) );
	auto accumulate_layers_target = reinterpret_cast< void* >( get_virtual( pt, 9 ) );
	auto do_extra_bone_processing_target = reinterpret_cast< void* >( get_virtual( pt, 197 ) );
	auto standard_blending_rules_target = reinterpret_cast< void* >( get_virtual( pt, 205 ) );
	auto update_client_side_animation_target = reinterpret_cast< void* >( get_virtual( pt, 223 ) );
	auto render_smoke_overlay_target = reinterpret_cast< void* >( get_virtual( interfaces::render_view, 41 ) );
	auto trace_ray_target = reinterpret_cast< void* >( get_virtual( interfaces::trace_ray, 8 ) );
	auto fire_event_target = reinterpret_cast< void* >( get_virtual( interfaces::engine, 59 ) ); // working
	auto eye_angles_target = reinterpret_cast< void* >( get_virtual( pt, 164 ) );
	auto override_view_target = reinterpret_cast< void* >( get_virtual( interfaces::clientmode, 18 ) );
	auto write_user_cmd_target = reinterpret_cast< void* >( get_virtual( interfaces::client, 24 ) );
	auto cl_move_target = reinterpret_cast< void* >( utilities::pattern_scan( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A" ) );
	auto run_commmand_target = reinterpret_cast< void* >( get_virtual( interfaces::prediction, 19 ) );
	auto crc_server_check_target = reinterpret_cast< void* >( utilities::pattern_scan( "engine.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80" ) );
	auto file_system_target = reinterpret_cast< void* >( get_virtual( interfaces::file_system, 128 ) );

	if ( MH_Initialize( ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize MH_Initialize." );
		return false;
	}

	if ( MH_CreateHook( frame_stage_target, &frame_stage::hook, reinterpret_cast< void** >( &frame_stage_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize frame_stage. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( crc_server_check_target, &crc_server_check::hook, reinterpret_cast< void** >( &crc_server_check_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize crc_server_check. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( create_move_target, &create_move::hook, reinterpret_cast< void** >( &create_move_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize create_move. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook( paint_traverse_target, &paint_traverse::hook, reinterpret_cast< void** >( &paint_traverse_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize paint_traverse. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook( post_screen_space_fx, &post_screen_space_fx::hook, reinterpret_cast< void** >( &post_screen_space_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize post_screen_space_fx. (outdated index?)" );
		return false;
	}

	if ( MH_CreateHook( draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast< void** >( &draw_model_execute_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize draw_model_execute. (outdated index?)" );
		return false;
	}

	/*if ( MH_CreateHook( scene_end_target, &scene_end::hook, reinterpret_cast< void** >( &scene_end_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize scene_end. (outdated index?)" );
		return false;
	}*/
	if ( MH_CreateHook( setup_bones_target, &setup_bones::hook, reinterpret_cast< void** >( &setup_bones_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize setup_bones. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( do_procedural_foot_plant_target, &do_procedural_foot_plant::hook, ( void** ) &do_procedural_foot_plant_original ) != MH_OK )
	{
		throw std::runtime_error( "failed to initialize prodcedural foot plant. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( do_extra_bone_processing_target, &do_extra_bone_processing::hook, ( void** ) &do_extra_bone_processing_original ) != MH_OK )
	{
		throw std::runtime_error( "failed to initialize do_extra_bone_processing (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( update_client_side_animation_target, &update_client_side_animation::hook, ( void** ) &update_client_side_animation_original ) != MH_OK )
	{
		throw std::runtime_error( "failed to initialize update client side. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( standard_blending_rules_target, &standard_blending_rules::hook, ( void** ) &standard_blending_rules_original ) != MH_OK )
	{
		throw std::runtime_error( "failed to initialize standard blending rules. (outdated index?)" );
		return false;
	}

	/*if ( MH_CreateHook( fire_event_target, &fire_event::hook, ( void** ) &fire_event_original ) != MH_OK )
	{
		throw std::runtime_error( "failed to initialize fire event. (outdated index?)" );
		return false;
	}*/
	if ( MH_CreateHook( override_view_target, &override_view::hook, reinterpret_cast< void** >( &override_view_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize override_view. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( write_user_cmd_target, &write_user_cmd::hook, reinterpret_cast< void** >( &write_user_cmd_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize write user cmd target. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( cl_move_target, &cl_move::hook, reinterpret_cast< void** >( &cl_move_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize cl move. (outdated index?)" );
		return false;
	}
	if ( MH_CreateHook( run_commmand_target, &run_commmand::hook, reinterpret_cast< void** >( &run_commmand_original ) ) != MH_OK ) {
		throw std::runtime_error( "failed to initialize run_commmand. (outdated index?)" );
		return false;
	}
	//if ( MH_CreateHook( trace_ray_target, &trace_ray::hook, ( void** ) &trace_ray_original ) != MH_OK )
	{
		//	throw std::runtime_error( "failed to initialize trace ray. (outdated index?)" );
		//	return false;
	}

	if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK ) {
		throw std::runtime_error( "failed to enable hooks." );
		return false;
	}

	events.setup( );


	console::log( "[setup] hooks initialized!\n" );
	return true;
}

void hooks::release( ) {
	variables::menu::opened = false;
	events.release( );

	MH_Uninitialize( );

	MH_DisableHook( MH_ALL_HOOKS );
}
inline void UpdateAnimationState222( anim_state* state, vec3_t ang ) {
	using fn = void( __vectorcall* )( void*, void*, float, float, float, void* );
	static auto ret = reinterpret_cast< fn >( utilities::pattern_scan( "client.dll", ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ) ) );

	if ( !ret || !state )
		return;

	ret( state, nullptr, 0.f, ang.y, ang.x, nullptr );
}
void __stdcall hooks::frame_stage::hook( int frame_stage ) {
	switch ( frame_stage ) {
	case FRAME_UNDEFINED:
		break;
	case FRAME_START:

		break;
	case FRAME_NET_UPDATE_START:
		shot_processor::manage_shots( );
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		resolver::frame_stage( );
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		player_manager::manage_bones( );


		break;
	case FRAME_NET_UPDATE_END:
		g_netdata.apply( );
		player_manager::setup_players( );
		break;
	case FRAME_RENDER_START:
		anti_aim::real_animation_fix( );
		static auto blur = interfaces::material_system->find_material( "dev/scope_bluroverlay", TEXTURE_GROUP_OTHER );
		blur->set_material_var_flag( material_var_no_draw, true );
		csgo::m_rate = ( int ) std::round( 1.f / interfaces::globals->interval_per_tick );
	//	misc::removals::remove_flash( );
//		misc::removals::remove_smoke( );
		
		break;
	case FRAME_RENDER_END:
		break;
	default:
		break;
	}

	frame_stage_original( interfaces::client, frame_stage );

	static int m_iLastCmdAck = 0;
	static float m_flNextCmdTime = 0.f;

	if ( csgo::local_player ) {
		int framstage_minus2 = frame_stage - 2;

		if ( framstage_minus2 ) {
			// do shit onetap does idk
		}
		else {
			if ( interfaces::clientstate && ( m_iLastCmdAck != interfaces::clientstate->m_last_command_ack || m_flNextCmdTime != interfaces::clientstate->m_flNextCmdTime ) )
			{
				if ( engine_prediction::m_stored_variables.m_flVelocityModifier != ( reinterpret_cast< uintptr_t >( csgo::local_player ) + 0xA38C ) )
				{
					*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( interfaces::prediction + 0x24 ) ) = 1;
					engine_prediction::m_stored_variables.m_flVelocityModifier = reinterpret_cast< uintptr_t >( csgo::local_player ) + 0xA38C;
				}

				m_iLastCmdAck = interfaces::clientstate->m_last_command_ack;
				m_flNextCmdTime = interfaces::clientstate->m_flNextCmdTime;
			}
		}
	}
	
}

void __fastcall hooks::override_view::hook( void* _this, void* _edx, view_setup_t* setup ) {
	if ( interfaces::engine->is_in_game( ) ) {
		if ( csgo::local_player && csgo::local_player->is_alive( ) && variables::visuals::fov > 0 )
			setup->fov = 90 + variables::visuals::fov;
	}

	misc::thirdperson::think( );

	override_view_original( interfaces::clientmode, _this, setup );
}
bool __fastcall hooks::create_move::hook( void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd ) {
	create_move_original( input_sample_frametime, cmd );

	if ( !cmd || !cmd->command_number )
		return create_move_original( input_sample_frametime, cmd );

	csgo::cmd = cmd;
	csgo::local_player = reinterpret_cast< player_t* >( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	if ( !interfaces::engine->is_in_game( ) )
		return  create_move_original( input_sample_frametime, cmd );
	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast< bool* >( *frame_pointer - 0x1C );
	send_packet = interfaces::engine->get_net_channel( )->choked_packets >= variables::antiaim::fakelag;

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	
	tickbase_system::pre_movement( );
	cmd->buttons |= in_bullrush;
	fake_lag::on_peek( cmd, send_packet );
	misc::movement::bunny_hop( cmd );
    misc::movement::strafe( cmd );

    autostop::autostop( cmd );
	autostop::auto_slow_down( cmd );
	autostop::slow_walk( cmd, false );
	nade_pred.trace( cmd );

	static bool choked_last_tick = false; static int tick_Choke = 0;
		resolver::create_move( cmd );
	engine_prediction::initialize( csgo::local_player, cmd ); {

		fake_lag::create_move( cmd, send_packet );
		anti_aim::on_create_move( cmd, send_packet );
		if ( send_packet ) {
			csgo::real_angle = cmd->viewangles;
			//interfaces::console->console_printf( "real YAW SENDED %f \n", csgo::real_angle.y );
		}
		if ( csgo::lagPeek )
			cmd->buttons ^= cmd_buttons::in_forward | cmd_buttons::in_back | cmd_buttons::in_moveleft | cmd_buttons::in_moveright;


		player_manager::setup_records( );
		if ( csgo::choke_next_tick )
		{
			send_packet = false;
			csgo::choke_next_tick = false;
		}

		ragebot::createmove( cmd, send_packet );
		csgo::m_tick = cmd->tick_count;
		//	legit_bot::think( cmd );
	} 	engine_prediction::unload( csgo::local_player );


	if ( tickbase_system::m_shift_data.m_should_attempt_shift && !tickbase_system::m_shift_data.m_should_disable ) {
		if ( csgo::m_goal_shift == 7 && ( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) {
			// are we IN_ATTACK?
			if ( cmd->buttons & in_attack ) {
				// remove the flag :D!
				cmd->buttons &= ~in_attack;
			}
		}
	}
	if ( cmd->buttons & in_attack ) {
		csgo::m_shot_command_number = cmd->command_number;
		csgo::m_shot_tickbase = csgo::local_player->get_tick_base( );
	}
	

	math::correct_movement( old_viewangles, cmd, old_forwardmove, old_sidemove );

	auto count = *( std::uint32_t* ) ( ( std::uint32_t ) csgo::local_player->get_renderable( ) + 0x2918 );

	std::memcpy( csgo::player_bones [ csgo::local_player->index( ) ], *( void** ) ( ( std::uint32_t ) csgo::local_player->get_renderable( ) + 0x290C ), sizeof( matrix_t ) * count );

	cmd->viewangles.angle_normalize( );
	cmd->viewangles.angle_clamp( );
	
	
		
	tickbase_system::post_movement( );


	// make sure to update our animations with the right angles.
	if ( cmd->command_number >= csgo::m_shot_command_number && csgo::m_shot_command_number >= cmd->command_number - interfaces::clientstate->m_choked_commands ) {
		csgo::m_angle = interfaces::input->m_commands [ csgo::m_shot_command_number % 150 ].viewangles;
	}
	else {
		csgo::m_angle = cmd->viewangles;
	}
	//REAL ANGLE WORKED - 22.342651 choked tics = 14 send_packet = TRUE
		//if ( interfaces::clientstate->m_choked_commands == variables::antiaim::fakelag && send_packet )
			//csgo::m_angle = cmd->viewangles;
	//interfaces::console->console_printf( "REAL ANGLE WORKED %f choked tics = %i send_packet = %s \n", csgo::m_angle.y, interfaces::clientstate->m_choked_commands, send_packet ? "TRUE" : "FALSE" );
	

	if ( tickbase_system::m_shift_data.m_should_attempt_shift && !tickbase_system::m_shift_data.m_should_disable ) {
		if ( csgo::m_goal_shift == 7 && ( tickbase_system::m_shift_data.m_prepare_recharge || tickbase_system::m_shift_data.m_did_shift_before && !tickbase_system::m_shift_data.m_should_be_ready ) ) {
			// are we IN_ATTACK?
			if ( cmd->buttons & in_attack ) {
				// remove the flag :D!
				cmd->buttons &= ~in_attack;
			}
		}
	}


	

	csgo::send_packet = send_packet;
	csgo::m_animate = true;
	csgo::m_update_fake = true;
	if ( tickbase_system::m_shift_data.m_should_attempt_shift && ( ( !tickbase_system::m_shift_data.m_should_be_ready && tickbase_system::m_shift_data.m_prepare_recharge ) || tickbase_system::m_shift_data.m_needs_recharge ))
		send_packet = true;
	csgo::fresh_tick = false;


	return false;
}

void __stdcall hooks::paint_traverse::hook( unsigned int panel, bool force_repaint, bool allow_force ) {
	auto panel_to_draw = fnv::hash( interfaces::panel->get_panel_name( panel ) );


	bool old_tickbase = tickbase_system::m_shift_data.m_should_attempt_shift;
	switch ( panel_to_draw ) {
	case fnv::hash( "MatSystemTopPanel" ):
	{
		render::draw_text_string( 10, 10, render::fonts::watermark_font, "csgo-cheat", false, color::white( 255 ) );

		visuals::think( );
		nade_pred.draw( );
		misc::hitmarker::think( );

		menu::render( );
		/*if ( interfaces::engine->is_connected( ) && interfaces::engine->is_in_game( ) && csgo::local_player && csgo::local_player->is_alive( ) )
		{
			vec3_t src3D, dst3D, forward, src, dst;
			trace_t tr;
			ray_t ray;
			trace_filter_skip_one_entity filter( csgo::local_player );

			forward = math::angle_vector( vec3_t( 0, csgo::real_angle.y, 0 ) );
			src3D = csgo::local_player->origin( );
			dst3D = src3D + ( forward * 50.f ); //replace 50 with the length you want the line to have

			ray.initialize( src3D, dst3D );

			interfaces::trace_ray->trace_ray( ray, 0, &filter, &tr );

			if ( !math::world_to_screen( src3D, src ) || !math::world_to_screen( tr.end, dst ) )
				return;

			render::draw_line( src.x, src.y, dst.x, dst.y, color::red( ) );

			forward = math::angle_vector( vec3_t( 0, resolver::server_feet_yaw( csgo::local_player ), 0 ) );
			dst3D = src3D + ( forward * 50.f ); //replace 50 with the length you want the line to have

			ray.initialize( src3D, dst3D );

			interfaces::trace_ray->trace_ray( ray, 0, &filter, &tr );

			if ( !math::world_to_screen( src3D, src ) || !math::world_to_screen( tr.end, dst ) )
				return;

			render::draw_line( src.x, src.y, dst.x, dst.y, color::green( ) );
			if ( !math::world_to_screen( src3D, src ) || !math::world_to_screen( tr.end, dst ) )
				return;

			render::draw_line( src.x, src.y, dst.x, dst.y, color::white( ) );




		}*/
	}
	break;

	case fnv::hash( "FocusOverlayPanel" ):
		interfaces::panel->set_keyboard_input_enabled( panel, variables::menu::opened );
		interfaces::panel->set_mouse_input_enabled( panel, variables::menu::opened );
		break;
	}

	csgo::m_goal_shift = variables::antiaim::db_tap_ticks;
	tickbase_system::m_shift_data.m_should_attempt_shift = variables::ragebot::double_tap;
	// (!) Temporary - replace with checkbox & hotkey later.
	if ( old_tickbase != tickbase_system::m_shift_data.m_should_attempt_shift ) {
		//g_notify.add( tfm::format( XOR( "Tried shifting tickbase" )) );

		if ( tickbase_system::m_shift_data.m_should_attempt_shift )
			tickbase_system::m_shift_data.m_needs_recharge = csgo::m_goal_shift;
		else
			tickbase_system::m_shift_data.m_needs_recharge = 0;

		tickbase_system::m_shift_data.m_did_shift_before = false;
	}

	static unsigned int panelID, panelHudID;

	if ( !panelHudID )
		if ( fnv::hash( "HudZoom" ) == panel_to_draw )
		{
			panelHudID = panel;
		}

	if ( panelHudID == panel && csgo::local_player && csgo::local_player->is_alive( ) )
	{
		if ( csgo::local_player->is_scoped( ) )
			return;
	}

	paint_traverse_original( interfaces::panel, panel, force_repaint, allow_force );
}

void __fastcall hooks::do_procedural_foot_plant::hook( void* a1, void* _edx, int a2, int a3, int a4, int a5 ) {
	auto orig = *( std::uint32_t* ) ( ( uintptr_t ) a1 + 96 );
	*( std::uint32_t* ) ( ( uintptr_t ) a1 + 96 ) = 0;

	__asm {
		mov ecx, a1
		push a5
		push a4
		push a3
		push a2
		call do_procedural_foot_plant_original
	}

	*( std::uint32_t* ) ( ( uintptr_t ) a1 + 96 ) = orig;
}
bool __fastcall hooks::post_screen_space_fx::hook( uintptr_t ecx, uintptr_t edx, const view_setup_t* setup ) {
	if ( !csgo::local_player )
		return post_screen_space_original( ecx, setup );

	visuals::glow::think( );

	return post_screen_space_original( ecx, setup );
}

void _fastcall hooks::do_extra_bone_processing::hook( void* ecx, uint32_t, void* hdr, vec3_t* pos, void* q, void* matrix, uint8_t* bone_computed, void* context ) {
	auto player = ( player_t* ) ecx;

	if ( player->m_fEffects( ) & 8 )
		return;

	const auto state = uint32_t( player->get_anim_state( ) );

	if ( !state )
		return do_extra_bone_processing_original( player, hdr, pos, q, matrix, bone_computed, context );

	const auto backup_tickcount = *reinterpret_cast< int32_t* >( state + 8 );
	*reinterpret_cast< int32_t* >( state + 8 ) = 0;
	do_extra_bone_processing_original( player, hdr, pos, q, matrix, bone_computed, context );
	*reinterpret_cast< int32_t* >( state + 8 ) = backup_tickcount;
}

void _fastcall hooks::standard_blending_rules::hook( player_t* player, uint32_t, c_studio_hdr* hdr, vec3_t* pos, quaternion_t* q, const float time, int mask )
{
	auto me = reinterpret_cast< player_t* > ( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( player && player->team( ) != me->team( ) && player != me )
		mask = BONE_USED_BY_SERVER;

	player->m_fEffects( ) |= 0x008;
	standard_blending_rules_original( player, hdr, pos, q, time, mask );
	player->m_fEffects( ) &= ~0x008;
}


void __fastcall hooks::update_client_side_animation::hook( player_t* player, uint32_t )
{
	const auto local = reinterpret_cast < player_t* > ( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( !local || !local->is_alive( ) )
		return update_client_side_animation_original( player );
	if ( !player )
		return update_client_side_animation_original( player );

	if ( !csgo::cmd )
		return update_client_side_animation_original( player );

	static int last_tickcount [ 65 ];

	if ( player->index( ) == interfaces::engine->get_local_player( ) ) {

		if ( csgo::m_update ) {
			update_client_side_animation_original( player );
		}
	//	if ( interfaces::globals->tick_count != last_tickcount [ player->index( ) ] && interfaces::engine && interfaces::engine->get_net_channel( ) && interfaces::engine->get_net_channel( )->choked_packets == 0 ) {

		/*	auto state = player->get_anim_state( ); if ( !state ) return update_client_side_animation_original( player );

			const float curtime = interfaces::globals->cur_time;
			const float frametime = interfaces::globals->frame_time;
			const float realtime = interfaces::globals->realtime;
			const float absoluteframetime = interfaces::globals->absolute_frametime;
			const float absoluteframestarttimestddev = interfaces::globals->absolute_frame_start_time;
			const float interpolation_amount = interfaces::globals->interpolation_amount;
			const float framecount = interfaces::globals->frame_count;
			const float tickcount = interfaces::globals->tick_count;

			static auto host_timescale = interfaces::console->get_convar( "host_timescale" );// ->find_var( ( "host_timescale" ) );

			interfaces::globals->cur_time = player->simulation_time( );
			interfaces::globals->realtime = player->simulation_time( );
			interfaces::globals->frame_time = interfaces::globals->interval_per_tick * host_timescale->get_float( );
			interfaces::globals->absolute_frametime = interfaces::globals->interval_per_tick * host_timescale->get_float( );
			interfaces::globals->absolute_frame_start_time = player->simulation_time( ) - interfaces::globals->interval_per_tick * host_timescale->get_float( );
			interfaces::globals->interpolation_amount = 0;
			interfaces::globals->frame_count = math::time_to_ticks( player->simulation_time( ) );
			interfaces::globals->tick_count = math::time_to_ticks( player->simulation_time( ) );

			int backup_flags = player->flags( );
			int backup_eflags = player->get_eflags( );

			animationlayer backup_layers [ 15 ];
			std::memcpy( backup_layers, player->get_anim_overlays( ), ( sizeof( animationlayer ) * 15 ) );

			if ( state->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count )
				state->m_iLastClientSideAnimationUpdateFramecount = interfaces::globals->frame_count - 1;

			update_client_side_animation_original( player );

			UpdateAnimationState( state, csgo::real_angle );
			float lby_delta = player->lower_body_yaw ( ) - csgo::real_angle.y;
			lby_delta = std::remainderf( lby_delta, 360.f );
			lby_delta = std::clamp( lby_delta, -60.f, 60.f );

			float feet_yaw = std::remainderf( csgo::real_angle.y + lby_delta, 360.f );

			if ( feet_yaw < 0.f ) {
				feet_yaw += 360.f;
			}


			std::memcpy( player->get_anim_overlays( ), backup_layers, ( sizeof( animationlayer ) * 15 ) );

		
			player->flags( ) = backup_flags;
			player->get_eflags( ) = backup_eflags;


			interfaces::globals->cur_time = curtime;
			interfaces::globals->realtime = realtime;
			interfaces::globals->frame_time = frametime;
			interfaces::globals->absolute_frametime = absoluteframetime;
			interfaces::globals->absolute_frame_start_time = absoluteframestarttimestddev;
			interfaces::globals->interpolation_amount = interpolation_amount;
			interfaces::globals->frame_count = framecount;
			interfaces::globals->tick_count = tickcount;

			
		*/
	//	}

	//	last_tickcount [ player->index( ) ] = interfaces::globals->tick_count;
		{
			

		}
	}
	else if ( player->simulation_time( ) != player->get_old_simulation_time( ) && interfaces::globals->tick_count != last_tickcount [ player->index( ) ] ) {
		auto index = player->index( );


		const float curtime = interfaces::globals->cur_time;
		const float frametime = interfaces::globals->frame_time;
		static auto host_timescale = interfaces::console->get_convar( ( "host_timescale" ) );

		interfaces::globals->frame_time = interfaces::globals->interval_per_tick * host_timescale->get_float( );
		interfaces::globals->cur_time = player->simulation_time( ) + interfaces::globals->interval_per_tick;

		vec3_t backup_velocity = player->velocity( );

		int backup_flags = player->flags( );
		int backup_eflags = player->get_eflags( );

		animationlayer backup_layers [ 15 ];
		std::memcpy( backup_layers, player->get_anim_overlays( ), ( sizeof( animationlayer ) * 15 ) );
		//player->get_clientside_animation( ) = true;
		update_client_side_animation_original( player );
		//	player->get_clientside_animation( ) = false;
	//	if ( player->get_anim_state( ) )
		//	player->get_anim_state( )->m_bOnGround?player->flags( ) |= ( 1 << 0 ):player->flags( ) &= ~( 1 << 0 );
	//	player->get_anim_state( )->m_flEyeYaw = player->eye_angles( ).y;

		player->get_eflags( ) &= ~0x1000;

		//entity->get_abs_velocity( ) = entity->get_velocity( );
		//entity->get_abs_velocity2( ) = entity->get_velocity( );
		//entity->unkpasted( ) = 0x1;



		float lby_delta = player->lower_body_yaw( ) - player->eye_angles( ).y;
		lby_delta = std::remainderf( lby_delta, 360.f );
		lby_delta = std::clamp( lby_delta, -60.f, 60.f );

		float feet_yaw = std::remainderf( player->eye_angles( ).y + lby_delta, 360.f );

		if ( feet_yaw < 0.f ) {
			feet_yaw += 360.f;
		}
		if ( player->get_anim_state( ) )
			if ( player->get_anim_state( )->m_iLastClientSideAnimationUpdateFramecount == interfaces::globals->frame_count )
				player->get_anim_state( )->m_iLastClientSideAnimationUpdateFramecount = interfaces::globals->frame_count - 1;
		static float pitch, yaw = 0.f;
		//	if ( player->get_anim_state( ) )
		//	player->get_anim_state( )->m_flGoalFeetYaw = player->get_anim_state( )->m_flCurrentFeetYaw = csgo::real_angle.y;


		std::memcpy( player->get_anim_overlays( ), backup_layers, ( sizeof( animationlayer ) * 15 ) );

		player->velocity( ) = backup_velocity;
		player->flags( ) = backup_flags;
		player->get_eflags( ) = backup_eflags;
		//if (csgo::local_player )
	//	csgo::local_player->set_local_viewangles( csgo::real_angle );
		interfaces::globals->cur_time = curtime;
	}
	last_tickcount [ player->index( ) ] = interfaces::globals->tick_count;
}

bool __fastcall hooks::setup_bones::hook( void* ecx, void* edx, matrix_t* bone_to_world_out, int max_bones, int bone_mask, float curtime ) {

	const auto local = reinterpret_cast < player_t* > ( interfaces::entity_list->get_client_entity( interfaces::engine->get_local_player( ) ) );

	if ( !local || !local->is_alive( ) )
		return  setup_bones_original( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
	auto e = reinterpret_cast < player_t* >( reinterpret_cast < uintptr_t > ( ecx ) - 0x4 );

	if ( !e )
		return setup_bones_original( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

	

	setup_bones_original( ecx, bone_to_world_out, max_bones, bone_mask, curtime );// math::get_estimate_server_time( csgo::cmd ) );

	

}
vec3_t* _fastcall hooks::eye_angles::hook( void* ecx, void* edx )
{
	if ( ecx != csgo::local_player )
		return eye_angles_original( ecx );

	static auto ret_to_thirdperson_pitch = utilities::pattern_scan( "client.dll", "8B CE F3 0F 10 00 8B 06 F3 0F 11 45 ? FF 90 ? ? ? ? F3 0F 10 55 ?" );
	static auto ret_to_thirdperson_yaw = utilities::pattern_scan( "client.dll", "F3 0F 10 55 ? 51 8B 8E ? ? ? ?" );

	if ( uintptr_t( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t >( ret_to_thirdperson_yaw ) || uintptr_t( _ReturnAddress( ) ) == reinterpret_cast< uintptr_t >( ret_to_thirdperson_yaw ) )
		return &csgo::real_angle;
}
bool __fastcall hooks::write_user_cmd::hook( void* ecx, void* edx, int m_nSlot, bf_write* m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd )
{
	if ( tickbase_system::m_shift_data.m_ticks_to_shift <= 0 )
		return write_user_cmd_original( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );
	//if (csgo::cmd && csgo::cmd->weaponselect != 0 )
		//return write_user_cmd_original( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, m_bNewCmd );
	if ( m_nFrom != -1 )
		return true;

	m_nFrom = -1;

	int m_nTickbase = tickbase_system::m_shift_data.m_ticks_to_shift;
	tickbase_system::m_shift_data.m_ticks_to_shift = 0;

	int* m_pnNewCmds = ( int* ) ( ( uintptr_t ) m_pBuffer - 0x2C );
	int* m_pnBackupCmds = ( int* ) ( ( uintptr_t ) m_pBuffer - 0x30 );

	*m_pnBackupCmds = 0;

	int m_nNewCmds = *m_pnNewCmds;
	int m_nNextCmd = interfaces::clientstate->m_choked_commands + interfaces::clientstate->m_last_outgoing_command + 1;
	int m_nTotalNewCmds = min( m_nNewCmds + abs( m_nTickbase ), 16 );

	*m_pnNewCmds = m_nTotalNewCmds;

	for ( m_nTo = m_nNextCmd - m_nNewCmds + 1; m_nTo <= m_nNextCmd; m_nTo++ ) {
		if ( !write_user_cmd_original( ecx, m_nSlot, m_pBuffer, m_nFrom, m_nTo, true ) )
			return false;

		m_nFrom = m_nTo;
	}

	c_usercmd* m_pCmd = interfaces::input->get_user_cmd( m_nSlot, m_nFrom );
	if ( !m_pCmd )
		return true;

	c_usercmd m_ToCmd = *m_pCmd, m_FromCmd = *m_pCmd;
	m_ToCmd.command_number++;
	m_ToCmd.tick_count += 3 * csgo::m_rate;
	

	interfaces::console->console_printf( "m_ToCMd_tick_count %i and m_rate is  %i \n", m_ToCmd.tick_count, csgo::m_rate );
	for ( int i = m_nNewCmds; i <= m_nTotalNewCmds; i++ ) {
		tickbase_system::write_user_cmd( ( bf_write* ) m_pBuffer, &m_ToCmd, &m_FromCmd );
		m_FromCmd = m_ToCmd;

		m_ToCmd.command_number++;
		m_ToCmd.tick_count++;
	}

	tickbase_system::m_shift_data.m_current_shift = m_nTickbase;
	return true;
}
void __fastcall  hooks::cl_move::hook( void* ecx )
{
	
		if ( tickbase_system::m_shift_data.m_should_attempt_shift && tickbase_system::m_shift_data.m_needs_recharge ) {
			--tickbase_system::m_shift_data.m_needs_recharge;

		   tickbase_system::m_shift_data.m_did_shift_before = false;

			if ( tickbase_system::m_shift_data.m_needs_recharge == 0 ) {
				tickbase_system::m_shift_data.m_should_be_ready = true;
			}

			return;
		}
	
	
	cl_move_original( ecx );
}
void _fastcall hooks::run_commmand::hook( void* prediction, void* edx, player_t* player, c_usercmd* cmd, player_move_helper* move_helper ) {
	//return run_commmand_original( interfaces::prediction, player, cmd, move_helper );
	if ( !player )
		return run_commmand_original( interfaces::prediction, player, cmd, move_helper );

	if ( player->index( ) != interfaces::engine->get_local_player( ) )
		return run_commmand_original( interfaces::prediction, player, cmd, move_helper );
	//interfaces::console->console_printf( "CMD->TICKCOUNT %i  calc %i csgo::m_tick %i\n", cmd->tick_count, ( csgo::m_tick + int( 1 / interfaces::globals->interval_per_tick ) + 8 ), csgo::m_tick );
	if ( cmd->tick_count >= ( csgo::m_tick + int( 1 / interfaces::globals->interval_per_tick ) + 8 ) ) {
		cmd->hasbeenpredicted = true;
		return;
	}
	int backup_tickbase = player->get_tick_base( );
	float backup_curtime = interfaces::globals->cur_time;

	if ( cmd->command_number == tickbase_system::m_prediction.m_shifted_command ) {
		player->get_tick_base( ) = ( tickbase_system::m_prediction.m_original_tickbase - tickbase_system::m_prediction.m_shifted_ticks + 1 );
		++player->get_tick_base( );

		interfaces::globals->cur_time = math::ticks_to_time( player->get_tick_base( ) );
	}
	float m_flVelModBackup = player->m_flVelocityModifier( );
	if ( csgo::m_update && cmd->command_number == interfaces::clientstate->m_last_command_ack + 1 )
		player->m_flVelocityModifier( ) = engine_prediction::m_stored_variables.m_flVelocityModifier;


	run_commmand_original( interfaces::prediction, player, cmd, move_helper );

	if ( !csgo::m_update )
		player->m_flVelocityModifier( ) = m_flVelModBackup;

	// restore tickbase and curtime.
	if ( cmd->command_number == tickbase_system::m_prediction.m_shifted_command ) {
		player->get_tick_base( ) = backup_tickbase;

		interfaces::globals->cur_time = backup_curtime;
	}

	g_netdata.store( );
}
void __fastcall hooks::fire_event::hook( void* ecx, void* edx )
{
	if ( !csgo::local_player || !interfaces::engine->is_in_game( ) || !interfaces::engine->is_connected( ) )
		return fire_event_original( ecx );




	/*	auto CL_ParseEventDelta = [ ] ( void* RawData, void* pToData, recv_table* pRecvTable )
		{
			// "RecvTable_DecodeZeros: table '%s' missing a decoder.", look at the function that calls it.
			static uintptr_t CL_ParseEventDeltaF = ( uintptr_t ) utilities::pattern_scan( ( "engine.dll" ), ( "55 8B EC 83 E4 F8 53 57" ) );
			__asm
			{
				mov     ecx, RawData
				mov     edx, pToData
				push	pRecvTable
				call    CL_ParseEventDeltaF
				add     esp, 8
			}
		};*/

		// Filtering events
	//if ( !csgo::local_player || !csgo::local_player->is_alive( ) || !variables::ragebot::enabled )
		return fire_event_original( ecx );


	struct event_t {
	public:
		uint8_t uchPad0 [ 4 ];
		float delay;
		uint8_t uchPad2 [ 48 ];
		event_t* next;
	};

	auto ei = *( event_t** ) ( std::uintptr_t( interfaces::clientstate ) + 0x4E64 );

	event_t* next = nullptr;

	if ( !ei )
		return fire_event_original( ecx );

	do {
		next = ei->next;
		ei->delay = 0.f;
		ei = next;
	} while ( next );
	return fire_event_original( ecx );
}
void __stdcall hooks::draw_model_execute::hook( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world ) {
	if ( !interfaces::engine->is_connected( ) || interfaces::engine->is_taking_screenshot( ) || interfaces::model_render->is_forced_material_override( ) )
		return draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );

	visuals::chams::run( ctx, state, info, bone_to_world );

	draw_model_execute_original( interfaces::model_render, ctx, state, info, bone_to_world );
	interfaces::model_render->override_material( NULL );
}
void __fastcall hooks::crc_server_check::hook( void* ecx, void* edx ) {
	return;
}

void __stdcall hooks::render_smoke_overlay::hook( void* ctx, bool a ) {
	render_smoke_overlay_original( ctx, a );
}
void __fastcall hooks::trace_ray::hook( void* thisptr, void*, const ray_t& ray, unsigned int fMask, trace_filter* pTraceFilter, trace_t* pTrace )
{

	if ( !csgo::in_trace )
		return trace_ray_original( thisptr, ray, fMask, pTraceFilter, pTrace );

	trace_ray_original( thisptr, ray, fMask, pTraceFilter, pTrace );

	pTrace->surface.flags |= SURF_SKY;
}
void __stdcall hooks::scene_end::hook( ) {
	


	scene_end_original( interfaces::render_view );
}

int __fastcall hooks::file_system::hook( void* ecx, void* edx ) {
	static auto return_to_client_panorama = utilities::pattern_scan( "client.dll", "83 F8 02 0F 85 ? ? ? ? 8B 3D ? ? ? ? FF D7" );
	static auto return_to_engine = utilities::pattern_scan( "engine.dll", "83 F8 02 75 6C 68 ? ? ? ? FF 15 ? ? ? ? 8B 4C 24 28 83 C4 04" );

	if ( _ReturnAddress( ) == return_to_client_panorama || _ReturnAddress( ) == return_to_engine )
		return 0; //return 0 in some cases to not being kicked from game

	return 1; //return 1 to allow loading of chams materials
}