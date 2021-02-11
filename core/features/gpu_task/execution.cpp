#include <deque>
#include "../features.hpp"
#include "../../menu/render/menu_render.hpp"

namespace gpu_task {
	size_t thread_lock = -1;

	std::vector<task*> tasklist;

	int last_task_id = 0;
	long get_mils ( ) {
		auto duration = std::chrono::system_clock::now ( ).time_since_epoch ( );
		return std::chrono::duration_cast< std::chrono::milliseconds >( duration ).count ( );
	}
	void task::wait_for ( ) {
		//if ( thread_lock != -1 )// && thread_lock != std::hash<std::thread::id>{}( std::this_thread::get_id ( ) ) )
		//	std::this_thread::sleep_for ( std::chrono::milliseconds ( 5 ) );

		//thread_lock = std::hash<std::thread::id> {}( std::this_thread::get_id ( ) );
		while ( !this->finished ) {
			/*if ( this->attemps > 5 ) {
				//this->handle ( );
				this->finished = true;
				interfaces::console->console_printf ( "GPU EXECUTION FAILED AND DID IT ON CPU \n" );
				return;
			}*/
			std::this_thread::sleep_for ( std::chrono::milliseconds ( 5 ) );
		
		}
		//thread_lock = -1;
	}
	void execute_tasks (  ) {
		//if ( thread_lock != -1 )// && thread_lock != std::hash<std::thread::id>{}( std::this_thread::get_id ( ) ) )
			//return;

		thread_lock = std::hash<std::thread::id> {}( std::this_thread::get_id ( ) );
		auto mil = get_mils ( );
	      
		for ( size_t i = tasklist.size(); i > 0; i-- ) {
			auto & current_task = tasklist.at ( i );
			if ( !current_task->finished ) {
				current_task->handle ( );
			}else if ( ( current_task->time - mil ) > 50 )
				tasklist.erase ( tasklist.begin ( ) + i );
		}
		thread_lock = -1;
		
	}
}