#pragma once

namespace visuals {

	class radar_entity {
		ImVec2 position;
		std::string name;
	};

	class radar_engine {
	private:
		void * window_target;


		float m_flPosX = 1.f;
		float m_flPosY = 1.f;
		float m_flScale = 1.f;
		float m_fMapScale = 1.f;
		vec3_t m_MapOrigin;
		float m_fZoom = 5.f;
		ImVec2 m_MapCenter = ImVec2 ( 512, 512 );
		IDirect3DTexture9 * mapTexture;
		std::array<float, 64> animation_progress;
	public:
		
		bool isValidMap = false;
		radar_engine ( );
		~radar_engine ( );

		void VectorYawRotate ( const ImVec2 & in, float flYaw, ImVec2 & out );


		ImVec2 WorldToMap ( const vec3_t & worldpos );

		void GetSize ( int & x, int & y );

		void GetPos ( int & x, int & y );

		ImVec2 MapToRadar ( const ImVec2 & mappos );

		void VectorYawRotate2 ( ImVec2 center, float angle, ImVec2 * p );

		void on_update ( );

	
		void on_map_load ( const char * pszMapName );

		void on_render ( );

	};


	extern radar_engine * radar;
}

