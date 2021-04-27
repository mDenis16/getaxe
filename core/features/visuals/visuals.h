

namespace visuals {

	void initialize ( );

	class box {
	public:
	
		float x, y, w, h;
		ImVec2 top_center;
		ImVec2 bottom_center;
		ImVec2 mins, maxs;

		box ( ) = default;
		box ( float x, float y, float w, float h ) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
	
	class visual_data {
	public:
		box box_data;
		entity_t * ent;
		bool ready = false;
		float distance = 0.f;
		int index;

		bool on_screen;
		bool valid;
		bool dormant;
	


		float last_seen_time = 0.f;
		vec3_t origin, mins, maxs;
	
		bool calculate_box ( );

		void virtual render_name ( ) = 0;
		void virtual render_box ( ) = 0;
		void virtual render_flags ( ) = 0;
		void virtual render_ammo ( ) = 0;
		void virtual render_health ( ) = 0;
		void virtual render_weapon ( ) = 0;
		void virtual render_entity ( ) = 0;
		void virtual render_offscreen ( ) = 0;
		void virtual queue_entity ( void* ent ) = 0;
	};


	class visual_player : public visual_data {
	public:
		std::string weapon_icon;
		std::string weapon_name;
		player_t * player;
		int health;
		bool alive;
		bool enemy;
		int type = 0;
		int ammo = 0;
		int money = 0;
		bool taze = false;
		bool kevlar = false;
		bool helmet = false;
		bool scoped = false;
		bool defuser = false;
		bool fake_duck = false;
		bool bomb_carrier = false;
		float alpha = 255;
		player_info_t player_info;
		bool out_of_pov = false;
		void render_name ( ) override;
		void render_box ( ) override;
		void render_flags ( ) override;
		void render_ammo ( ) override;
		void render_health ( ) override;
		void render_weapon ( ) override;
		void render_entity ( ) override;
		void render_offscreen ( ) override;
		void queue_entity ( void * entity ) override;
		void handle_flags ( );
	};


	enum font_align {
		font_left,
		font_center,
		font_right
	};

	void on_render ( );
	void on_queue ( );

	void primitive_string ( bool borderedl, ID3DXFont * font, float x, float y, ImColor _color, ImColor background, int orientation, const char * input, ... );
	bool world_to_screen ( const vec3_t & origin, ImVec2 & screen );
	void rotate_triangle ( std::array<ImVec2, 3> & points, float rotation );

	inline ImDrawList * render = nullptr;

	enum fonts {
		ESP,
		LOGS,
		FLAGS,
		GRENADES
	};
	inline std::array< ID3DXFont *, 4> fonts;
	inline std::array< visual_player, 64> player_data;
	inline IDirect3DDevice9 * device;
	inline ImFont * esp_font;
	inline ImFont * weapon_font;

	inline std::map<int, char> weapon_icons =
	{
	{ weapon_deagle, 'A' },
	{ weapon_elite, 'B' },
	{ weapon_fiveseven, 'C' },
	{ weapon_glock, 'D' },
	{ weapon_hkp2000, 'E' },
	{ weapon_p250, 'F' },
	{ weapon_usp_silencer, 'G' },
	{ weapon_tec9, 'H' },
	{ weapon_cz75a, 'I' },
	{ weapon_revolver, 'J' },
	{ weapon_mac10, 'K' },
	{ weapon_ump45, 'L' },
	{ weapon_bizon, 'M' },
	{ weapon_mp7, 'N' },
	{ weapon_mp5sd, 'L' },
	{ weapon_mp9, 'O' },
	{ weapon_p90, 'P' },
	{ weapon_galilar, 'Q' },
	{ weapon_famas, 'R' },
	{ weapon_m4a1, 'S' },
	{ weapon_m4a1_silencer, 'T' },
	{ weapon_aug, 'U' },
	{ weapon_sg556, 'V' },
	{ weapon_ak47, 'W' },
	{ weapon_g3sg1, 'X' },
	{ weapon_scar20, 'Y' },
	{ weapon_awp, 'Z' },
	{ weapon_ssg08, 'a' },
	{ weapon_xm1014, 'b' },
	{ weapon_sawedoff, 'c' },
	{ weapon_mag7, 'd' },
	{ weapon_nova, 'e' },
	{ weapon_negev, 'f' },
	{ weapon_m249, 'g' },
	{ weapon_taser, 'h' },
	{ weapon_knife_t, 'i' },
	{ weapon_knife, 'j' },
	{ weapon_knife_falchion, '0' },
	{ weapon_knife_bayonet, '1' },
	{ weapon_knife_flip, '2' },
	{ weapon_knife_gut, '3' },
	{ weapon_knife_karambit, '4' },
	{ weapon_knife_m9_bayonet, '5' },
	{ weapon_knife_tactical, '6' },
	{ weapon_knife_bowie, '7' },
	{ weapon_knife_butterfly, '8' },
	{ weapon_knife_ursus, 'j' },
	{ weapon_knife, 'j' },
	{ weapon_knife_stiletto, 'j' },
	{ weapon_knife_widowmaker, 'j' },
	{ weapon_flashbang, 'k' },
	{ weapon_hegrenade, 'l' },
	{ weapon_smokegrenade, 'm' },
	{ weapon_molotov, 'n' },
	{ weapon_decoy, 'o' },
	{ weapon_incgrenade, 'p' },
	{ weapon_c4, 'q' },
	};
}