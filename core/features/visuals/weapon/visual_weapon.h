#pragma once
namespace visuals {
	class visual_weapon_t : public visual_data {
	public:
		weapon_t * weapon;
		weapon_visual * cfg;
		int ammo = 0;
		int clip = 0;
		int max_clip = 0;
		std::string weapon_icon;
		void render_name ( ) ;
		void render_box ( ) ;
		void render_ammo ( ) ;
		void on_render ( ) override;
		void on_update (  ) override;
	};
}

