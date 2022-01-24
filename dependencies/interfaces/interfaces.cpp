#include "interfaces.hpp"
#include "../utilities/csgo.hpp"
#include "../utilities/obf.h"


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
virtual_game_movement* interfaces::game_movement = nullptr;
player_prediction* interfaces::prediction = nullptr;
player_move_helper* interfaces::move_helper = nullptr;
i_weapon_system* interfaces::weapon_system = nullptr;
i_hud_chat* interfaces::chat_element = nullptr;
c_physics_api* interfaces::physics = nullptr;
iv_engine_sound* interfaces::engine_sound = nullptr;
IMemAlloc* interfaces::memalloc = nullptr;
IMDLCache* interfaces::modelcache = nullptr;
c_csplayer_resource* interfaces::player_resource = nullptr;
i_view_render_beams* interfaces::render_beams = nullptr;

void* interfaces::file_system = nullptr;

#pragma optimize( "", off )

#define UL unsigned long
#define znew  ((z=36969*(z&65535)+(z>>16))<<16)
#define wnew  ((w=18000*(w&65535)+(w>>16))&65535)
#define MWC   (znew+wnew)
#define SHR3  (jsr=(jsr=(jsr=jsr^(jsr<<17))^(jsr>>13))^(jsr<<5))
#define CONG  (jcong=69069*jcong+1234567)
#define KISS  ((MWC^CONG)+SHR3)


static UL z = 362436069 * (int)__TIMESTAMP__, w = 521288629 * (int)__TIMESTAMP__, \
jsr = 123456789 * (int)__TIMESTAMP__, jcong = 380116160 * (int)__TIMESTAMP__;



#pragma optimize( "", off )
bool interfaces::initialize() {




	//client = f(); // TEST_SAMP<i_base_client_dll>(interfaces_manager::INTERFACES_ADDRESS[interfaces_manager::GET_INTERFACE_HASH<"VClient018">()]);


	

	    FETCH_INTERFACE(client, i_base_client_dll, "VClient018")

		FETCH_INTERFACE(entity_list, i_client_entity_list, "VClientEntityList003")

		FETCH_INTERFACE(engine, iv_engine_client, "VEngineClient014")

		FETCH_INTERFACE(engine_sound, iv_engine_sound, "IEngineSoundClient003")

		FETCH_INTERFACE(panel, i_panel, "VGUI_Panel009")

		FETCH_INTERFACE(surface, i_surface, "VGUI_Surface031")

		FETCH_INTERFACE(material_system, i_material_system, "VMaterialSystem080")

		FETCH_INTERFACE(model_info, iv_model_info, "VModelInfoClient004")

		FETCH_INTERFACE(model_render, iv_model_render, "VEngineModel016")

		FETCH_INTERFACE(render_view, i_render_view, "VEngineRenderView014")

		FETCH_INTERFACE(console, i_console, "VEngineCvar007")

		FETCH_INTERFACE(localize, i_localize, "Localize_001")

		FETCH_INTERFACE(event_manager, i_game_event_manager, "GAMEEVENTSMANAGER002")

		FETCH_INTERFACE(debug_overlay, iv_debug_overlay, "VDebugOverlay004")

		FETCH_INTERFACE(inputsystem, i_inputsytem, "InputSystemVersion001")

		//FETCH_INTERFACE(game_movement, player_game_movement, "GameMovement001")

		FETCH_INTERFACE(prediction, player_prediction, "VClientPrediction001")

		FETCH_INTERFACE(physics, c_physics_api, "VPhysicsSurfaceProps001")

		FETCH_INTERFACE(trace_ray, i_engine_trace, "EngineTraceClient004")

	    FETCH_INTERFACE(modelcache, IMDLCache, "MDLCache004")


		chat_element = hud_element<i_hud_chat>(("CHudChat"));



		/*custom interfaces*/
		clientmode = **reinterpret_cast<i_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);

	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);


	clientstate = **(i_client_state***)(utilities::pattern_scan("engine.dll", sig_client_state) + 1);

	directx = **(IDirect3DDevice9***)(utilities::pattern_scan("shaderapidx9.dll", sig_directx) + 1);

	input = *(i_input**)(utilities::pattern_scan("client.dll", sig_input) + 1);

	glow_manager = (glow_manager_t*)(*(uintptr_t*)(utilities::pattern_scan("client.dll", sig_glow_manager) + 3));

	move_helper = **(player_move_helper***)(utilities::pattern_scan("client.dll", sig_player_move_helper) + 0x2);

	weapon_system = *(i_weapon_system**)(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);

	file_system = get_interface<void, interface_type::index>("filesystem_stdio.dll", "VFileSystem017");

	memalloc = *reinterpret_cast<IMemAlloc**>(GetProcAddress(GetModuleHandle(("tier0.dll")), ("g_pMemAlloc")));

	player_resource = **reinterpret_cast<c_csplayer_resource***>(utilities::pattern_scan(crypt_str("client.dll"), "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 0x2);

	render_beams = *(i_view_render_beams**)(utilities::pattern_scan("client.dll", "B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9") + 0x1);

	game_movement = get_interface<virtual_game_movement, interface_type::index>("client.dll", "GameMovement001");


	console::log("[setup] interfaces initialized!\n");


	return true;
}

#pragma optimize( "", on ) 