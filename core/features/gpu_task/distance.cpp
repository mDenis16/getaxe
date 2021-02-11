#include <deque>
#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"

namespace gpu_task {

	calc_distance::calc_distance ( vec3_t _from, vec3_t _to, bool _calc_2d ) {
		this->id = last_task_id;
		this->from = _from;
		this->to = _to;
		this->calc_2d = _calc_2d;
		last_task_id++;
		//if ( thread_lock != -1 && thread_lock != std::hash<std::thread::id>{}( std::this_thread::get_id ( ) ) )
		//	std::this_thread::sleep_for ( std::chrono::milliseconds ( 5 ) );

		thread_lock = std::hash<std::thread::id> {}( std::this_thread::get_id ( ) );
		tasklist.push_back ( this );
		thread_lock = -1;
	}
	void calc_distance::handle ( ) {
	
		 result = std::sqrtf ( std::powf ( ( from.y - to.y ), 2 ) + powf ( ( from.x - to.x ), 2 ) + ( calc_2d ? 1.0f : std::powf ( ( from.z - to.z ), 2 ) ) );
		 this->finished = true;
	
	}
}