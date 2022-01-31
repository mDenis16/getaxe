#include "../hooks.hpp"
namespace hooks::callback {
	void __cdecl physics_simulate( entity_t* entity ) {

		//std::cout << "Phyisics Simulate callback " << std::endl;

		return physics_simulate_original(entity);
	}
}