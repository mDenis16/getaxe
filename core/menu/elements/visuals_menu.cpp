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
		
		static float color [ 4 ] = { 255, 0,0, 255 };

		option ( "Enemy Assist", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Name", "Draws a name on top of head", variables::visuals::enemy::name, variables::visuals::enemy::name_color );
			option_single ( "Box", "Draws a box on enemy", variables::visuals::enemy::box, variables::visuals::enemy::box_color );
			option_single ( "Health", "Draw health at left ", variables::visuals::enemy::health, variables::visuals::enemy::health_color );
			option_single ( "Weapon", "Draw weapon of player ", variables::visuals::enemy::weapon, variables::visuals::enemy::weapon_color );
		}, 400, variables::visuals::enemy::enabled );

		option ( "Team Assist", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Name", "Draws a name on top of head", variables::visuals::team::name, variables::visuals::team::name_color );
			option_single ( "Box", "Draws a box on team", variables::visuals::team::box, variables::visuals::team::box_color );
			option_single ( "Health", "Draw health at left ", variables::visuals::team::health, variables::visuals::team::health_color );
			option_single ( "Weapon", "Draw weapon of player ", variables::visuals::team::weapon, variables::visuals::team::weapon_color );
		}, 400, variables::visuals::team::enabled );

		option ( "Enemy Modulation", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Visible", "Draw player visible", variables::visuals::modulation::enemy::visible, variables::visuals::modulation::enemy::visible_color);
			option_single ( "XYZ", "Draw player invisible", variables::visuals::modulation::enemy::xyz, variables::visuals::modulation::enemy::xyz_color );
			option_single ( "Glow", "Draw player visible", variables::visuals::modulation::enemy::glow, variables::visuals::modulation::enemy::glow_color );
		}, 300, variables::visuals::modulation::enemy::enabled );
		option ( "Team Modulation", "Shows you enemyes on esp", [ = ] ( ) {
			option_single ( "Visible", "Draw player visible", variables::visuals::modulation::team::visible, variables::visuals::modulation::team::visible_color );
			option_single ( "XYZ", "Draw player invisible", variables::visuals::modulation::team::visible, variables::visuals::modulation::team::visible_color );
			option_single ( "Glow", "Draw player visible", variables::visuals::modulation::team::glow, variables::visuals::modulation::team::glow_color );
		}, 300, variables::visuals::modulation::team::enabled );
		option ( "World Assist", "Assist your gameplay", [ = ] ( ) {
			option_single ( "Grenade prediction", "Predict your grenade path", variables::visuals::world::grenade_prediction, variables::visuals::world::grenade_prediction_color );
			
			option_single ( "Weapons esp", "Show weapons on map", variables::visuals::world::draw_weapons, variables::visuals::world::draw_weapons_color );
			option_single ( "Grenades esp", "Show grenades on map", variables::visuals::world::draw_grenades, variables::visuals::world::draw_weapons_color );
		}, 300, variables::visuals::modulation::team::enabled );

	}
