#pragma once

namespace visuals {


	struct bone_data {
		vec3_t child;
		vec3_t parent;
	};
	struct flag_struct {
		ImColor color;
		std::string flag;
	};


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
		entity_t * entity;
		bool ready = false;
	
		int class_id = -1;
		int index;
		bool mark_deletetion = false;

		bool already_predicted = false;

		bool on_screen;
		bool valid;
		bool dormant;
		int distance = 0;
		int idx = 0;

		float last_seen_time = 0.f;
		vec3_t origin, mins, maxs;

		bool calculate_box ( );

		void delete_entity ( );


		void virtual on_render ( ) = 0;
		void virtual on_update ( ) = 0;

	};
}