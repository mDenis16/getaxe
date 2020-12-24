#include "../menu.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/impl/imgui_impl_dx9.h"
#include "../ImGui/impl/imgui_impl_win32.h"
#include "../config.hpp"

void menu::visuals ( ) {
	option ( "Enemy Assist", "Shows you enemyes on esp", [ = ] ( ) {
		option_single ( "Name", "Draws a name on top of head", config.visuals_enemy_name, config.visuals_enemy_name_color );
		option_single ( "Box", "Draws a box on enemy", config.visuals_enemy_box, config.visuals_enemy_box_color );
		option_single ( "Health", "Draw health at left ", config.visuals_enemy_health, config.visuals_enemy_health_color );
		option_single ( "Weapon", "Draw weapon of player ", config.visuals_enemy_weapon, config.visuals_enemy_weapon_color );
	}, 400, config.visuals_enemy_enabled );

	option ( "Team Assist", "Shows you enemyes on esp", [ = ] ( ) {
		option_single ( "Name", "Draws a name on top of head", config.visuals_team_name, config.visuals_team_name_color );
		option_single ( "Box", "Draws a box on team", config.visuals_team_box, config.visuals_team_box_color );
		option_single ( "Health", "Draw health at left ", config.visuals_team_health, config.visuals_team_health_color );
		option_single ( "Weapon", "Draw weapon of player ", config.visuals_team_weapon, config.visuals_team_weapon_color );
	}, 400, config.visuals_team_enabled );

	 option ( "Enemy Modulation", "Shows you enemyes on esp", [ = ] ( ) {
		option_single ( "Visible", "Draw player visible", config.visuals_modulation_enemy_visible, config.visuals_modulation_enemy_visible_color );
		option_single ( "XYZ", "Draw player invisible", config.visuals_modulation_enemy_xyz, config.visuals_modulation_enemy_xyz_color );
		option_combobox ( "Material", "Set your preffered modulation material", std::vector<std::string>{"solid", "flat", "reflective", "glow"}, config.visuals_modulation_enemy_material );
		option_single ( "Lag Compensation", "Draw player lag comp", config.visuals_modulation_enemy_lagcompensation, config.visuals_modulation_enemy_lagcompensation_color );
		option_single ( "Glow", "Draw player visible", config.visuals_modulation_enemy_glow, config.visuals_modulation_enemy_glow_color );
	}, 320, config.visuals_modulation_enemy_enabled );
	option ( "Team Modulation", "Shows you enemyes on esp", [ = ] ( ) {
		option_single ( "Visible", "Draw player visible", config.visuals_modulation_team_visible, config.visuals_modulation_team_visible_color );
		option_single ( "XYZ", "Draw player invisible", config.visuals_modulation_team_visible, config.visuals_modulation_team_visible_color );
		option_combobox ( "Material", "Set your preffered modulation material", std::vector<std::string>{"solid", "flat", "glow"}, config.visuals_modulation_team_material );
		option_single ( "Glow", "Draw player visible", config.visuals_modulation_team_glow, config.visuals_modulation_team_glow_color );
	}, 300, config.visuals_modulation_team_enabled );
	option ( "Local Modulation", "Shows you enemyes on esp", [ = ] ( ) {
		option_single ( "Weapons", "Modulate weapon", config.visuals_modulation_local_weapon, config.visuals_modulation_local_weapon_color );
		option_combobox ( "Weapon Material", "Set your preffered modulation material", std::vector<std::string>{"solid", "flat", "reflective", "glow"}, config.visuals_modulation_local_weapon_material );

		option_single ( "Arms", "Modulate arms", config.visuals_modulation_local_weapon, config.visuals_modulation_local_arms_color );
		option_combobox ( "Arms Material", "Set your preffered modulation material", std::vector<std::string>{"solid", "flat", "reflective", "glow"}, config.visuals_modulation_local_arms_material );
		
	}, 300, config.visuals_modulation_local_enabled );
	option ( "World Assist", "Assist your gameplay", [ = ] ( ) {
		option_single ( "Grenade prediction", "Predict your grenade path", config.visuals_world_grenade_prediction, config.visuals_world_grenade_prediction_color );

		option_single ( "Weapons esp", "Show weapons on map", config.visuals_world_draw_weapons, config.visuals_world_draw_weapons_color );
		option_single ( "Grenades esp", "Show grenades on map", config.visuals_world_draw_grenades, config.visuals_world_draw_weapons_color );
		option_single ( "Grenades Proximity", "Show grenades that hit you", config.visuals_world_draw_grenades_proximity, config.visuals_world_draw_weapons_color );
		
	}, 300, config.visuals_modulation_team_enabled );
}