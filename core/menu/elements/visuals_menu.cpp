#include "../menu.hpp"
#include "../../menu/variables.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/impl/imgui_impl_dx9.h"
#include "../ImGui/impl/imgui_impl_win32.h"

	void menu::visuals( ) {
		static bool asd = false;
		static bool asd2 = false;
		static bool asd3 = false;
		
		static float color [ 3 ] = { 255, 0,0 };

		option ( "Enemy Assist", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Name", "Draws a name on top of head", variables::visuals::enemy::name, static_cast< void * >( color ) );
			option_single ( "Box", "Draws a box on enemy", variables::visuals::enemy::box, static_cast< void * >( color ) );
			option_single ( "Health", "Draw health at left ", variables::visuals::enemy::health, static_cast< void * >( color ) );
			option_single ( "Weapon", "Draw weapon of player ", variables::visuals::enemy::weapon, static_cast< void * >( color ) );
		}, 400, variables::visuals::enemy::enabled );
		option ( "Team Assist", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Name", "Draws a name on top of head", variables::visuals::team::name, static_cast< void * >( color ) );
			option_single ( "Box", "Draws a box on enemy", variables::visuals::team::box, static_cast< void * >( color ) );
			option_single ( "Health", "Draw health at left ", variables::visuals::team::health, static_cast< void * >( color ) );
			option_single ( "Weapon", "Draw weapon of player ", variables::visuals::team::weapon, static_cast< void * >( color ) );
		}, 400, variables::visuals::team::enabled );
		option ( "Enemy Modulation", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Visible", "Draw player visible", variables::visuals::modulation::enemy::visible, static_cast< void * >( color ) );
			option_single ( "XYZ", "Draw player visible", variables::visuals::modulation::enemy::visible, static_cast< void * >( color ) );
			option_single ( "Glow", "Draw player visible", variables::visuals::modulation::enemy::glow, static_cast< void * >( color ) );
		}, 300, variables::visuals::modulation::enemy::enabled );
		option ( "Team Modulation", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Visible", "Draw player visible", variables::visuals::modulation::team::visible, static_cast< void * >( color ) );
			option_single ( "XYZ", "Draw player visible", variables::visuals::modulation::team::visible, static_cast< void * >( color ) );
			option_single ( "Glow", "Draw player visible", variables::visuals::modulation::team::glow, static_cast< void * >( color ) );
		}, 300, variables::visuals::modulation::team::enabled );


	}
