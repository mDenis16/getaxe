#include "interfaces.hpp"
#include "../utilities/csgo.hpp"

i_base_client_dll* interfaces::client = nullptr;
i_input* interfaces::input = nullptr;
i_client_entity_list* interfaces::entity_list = nullptr;
iv_engine_client* interfaces::engine = nullptr;
i_client_mode* interfaces::clientmode = nullptr;
i_client_state* interfaces::clientstate = nullptr;
i_panel* interfaces::panel = nullptr;
i_surface* interfaces::surface = nullptr;
c_global_vars_base* interfaces::globals = nullptr;
i_material_system* interfaces::material_system = nullptr;
iv_model_info* interfaces::model_info = nullptr;
iv_model_render* interfaces::model_render = nullptr;
i_render_view* interfaces::render_view = nullptr;
i_console* interfaces::console = nullptr;
i_localize* interfaces::localize = nullptr;
i_game_event_manager* interfaces::event_manager = nullptr;
i_inputsytem* interfaces::inputsystem = nullptr;
iv_debug_overlay* interfaces::debug_overlay = nullptr;
IDirect3DDevice9* interfaces::directx = nullptr;
i_engine_trace* interfaces::trace_ray = nullptr;
glow_manager_t* interfaces::glow_manager = nullptr;
player_game_movement* interfaces::game_movement = nullptr;
player_prediction* interfaces::prediction = nullptr;
player_move_helper* interfaces::move_helper = nullptr;
i_weapon_system* interfaces::weapon_system = nullptr;
i_hud_chat* interfaces::chat_element = nullptr;
c_physics_api* interfaces::physics = nullptr;
iv_engine_sound * interfaces::engine_sound = nullptr;
IMemAlloc * interfaces::memalloc = nullptr;
c_csplayer_resource * interfaces::player_resource = nullptr;
i_view_render_beams * interfaces::render_beams = nullptr;
void* interfaces::model_cache = nullptr;
void* interfaces::file_system = nullptr;

bool interfaces::initialize() {


	client = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");

	printf("INTERFACE: client %p \n", client);

	entity_list = get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");

	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");

	engine_sound = get_interface<iv_engine_sound, interface_type::index> ( "engine.dll", "IEngineSoundClient003" );

	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");

	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");

	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");

	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");

	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");

	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");

	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");

	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");

	event_manager = get_interface<i_game_event_manager, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");

	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");

	inputsystem = get_interface<i_inputsytem, interface_type::index>("inputsystem.dll", "InputSystemVersion001");

	game_movement = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");

	prediction = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");


	physics = ( c_physics_api* ) utilities::grab_interface( "vphysics.dll", "VPhysicsSurfaceProps", true );

	trace_ray = ( i_engine_trace* ) utilities::grab_interface( "engine.dll", "EngineTraceClient", true );

	chat_element = hud_element<i_hud_chat>( ( "CHudChat" ) );

	model_cache = reinterpret_cast< void* >( utilities::grab_interface( "datacache.dll", "MDLCache", true ) );

	
	/*custom interfaces*/
	clientmode = **reinterpret_cast<i_client_mode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 5);

	globals = **reinterpret_cast< c_global_vars_base*** >( ( *reinterpret_cast< uintptr_t** >( client ) ) [ 11 ] + 10 );

	clientstate = **(i_client_state ***)(utilities::pattern_scan("engine.dll", sig_client_state) + 1);

	directx = **(IDirect3DDevice9***)(utilities::pattern_scan("shaderapidx9.dll", sig_directx) + 1);

	input = *(i_input**)(utilities::pattern_scan("client.dll", sig_input) + 1);

	glow_manager = (glow_manager_t*)(*(uintptr_t*)(utilities::pattern_scan("client.dll", sig_glow_manager) + 3));

	move_helper = **(player_move_helper***)(utilities::pattern_scan("client.dll", sig_player_move_helper) + 2);

	weapon_system = *(i_weapon_system**)(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);

	file_system = get_interface<void, interface_type::index>( "filesystem_stdio.dll", "VFileSystem017" );

	memalloc = *reinterpret_cast< IMemAlloc ** >( GetProcAddress ( GetModuleHandle (  ( "tier0.dll" ) ),  ( "g_pMemAlloc" ) ) );

	player_resource = **reinterpret_cast< c_csplayer_resource *** >( utilities::pattern_scan ( crypt_str ( "client.dll" ), "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7" ) + 0x2 );

	render_beams = *( i_view_render_beams ** ) ( utilities::pattern_scan ( "client.dll", "B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9" ) + 0x1 );
	

	console::log("[setup] interfaces initialized!\n");

	return true;
}