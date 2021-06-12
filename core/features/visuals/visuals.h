#pragma once
#include "../visuals/visual_entities.h"
#include "../visuals/player/visual_player.h"
#include "../visuals/projectile/visual_projectile.h"
#include "../visuals/weapon/visual_weapon.h"
#include "../visuals/grenade/visual_grenade.h"
#include "../visuals/radar/radar.h"
#include "../../../dependencies/interfaces/i_client_entity_list.hpp"

namespace visuals {

	


	namespace removals {
		void remove_flash ( );
		void remove_smoke ( );
	}

	void thirdperson ( );




	enum font_align {
		font_left,
		font_center,
		font_right
	};

	void rotate_circle ( std::vector<ImVec2> & points, ImVec2 center, float rotation );
	void rotate_point ( ImVec2 & point, ImVec2 center, float rotation );
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
	inline std::array< visual_weapon_t, 64> weapon_data;
	inline std::array< visual_projectile, 64> projectile_data;

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

	namespace grenade_prediction {


		void Simulate ( entity_t * last_hit_player, grenade_t * grenade, std::vector<vec3_t> & path, float elasticty, int type, vec3_t * vecSrc );
		int Step ( entity_t * last_hit_player, grenade_t * grenade, vec3_t & vecSrc, vec3_t & vecThrow, float elasticity, int tick, float interval, int type );
	
		bool CheckDetonate ( grenade_t * grenade, const vec3_t & vecThrow, const trace_t & tr, int tick, float interval, int type );
		void TraceHull ( grenade_t * grenade, vec3_t & src, vec3_t & end, trace_t & tr );
		void AddGravityMove ( grenade_t * grenade, vec3_t & move, vec3_t & vel, float frametime, bool onground );
		void PushEntity ( grenade_t * grenade, vec3_t & src, const vec3_t & move, trace_t & tr );
		void ResolveFlyCollisionCustom ( entity_t * last_hit_player,  grenade_t * grenade, trace_t & tr, float elasticity, vec3_t & vecVelocity, float interval );

		int PhysicsClipVelocity ( grenade_t * grenade, const vec3_t & in, const vec3_t & normal, vec3_t & out, float overbounce );

	}


	class c_handler :  public i_client_entity_listener {
	public:
		void on_entity_created ( void * ent ) override;
		void on_entity_deleted ( void * ent ) override;

		void init ( );
		void remove ( );

		void on_update ( );
	
		void on_render ( );

		void local_player ( );
		void intialization ( );

		std::vector<visual_data *> entity_list;
		std::vector<entity_t *> grenade_list;

		//std::array< container_t, 64 > m_track;
	};

	extern c_handler * handler;


}