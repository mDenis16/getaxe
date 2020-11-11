#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <functional>
#include <iostream>
namespace menu {
	
	void start( IDirect3DDevice9* device );

	void option_slider ( const char * name, const char * description, float min, float max, float & value );

	void option_slider_int ( const char * name, const char * description, int min, int max, int & value );

	struct combobox_select_item { std::string name; bool * value; };





	void option_multicombobox ( const char * name, const char * description, std::vector<combobox_select_item> multi_items );

	void option_combobox ( const char * name, const char * description, std::vector<std::string> items, int & value );

	
	void option_combobox ( const char * name, const char * description, char * items[] , int & value );

	void option_single ( const char * name, const char * description, bool & value, float color [ 4 ] = nullptr );




	void option ( const char * name, const char * description, std::function<void ( )> func, int max_height, bool & value, float color [ 4 ] = nullptr);

	void ragebot ( );


	void render_menu( );

	void render_visuals( );

	

	
	void aimbot( );
	void visuals( );

	inline bool init_style = false;

	struct menu_data {
		 int width = 233;
		 bool show_tabs_buttons = false;
		 int width_transition = 50;
		 int width_tabs = 50;
		 int option_index = 0;
	};
	extern menu_data m_menu_data;

	struct option_data {
		 int max_height = 310;
		 int height = 65;
		 int cur_height = height;
		 bool enable = false;
		 bool in_transition = false;
	};
	extern  BYTE left_arrow [ ];
	extern IDirect3DTexture9 * buttonimage;
	extern option_data m_option_data[100];
}
