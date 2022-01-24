#include "../../features/features.hpp"

void lag_compensation::frame_update_post_entity_think()
{
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		lag_entity* entity = reinterpret_cast<lag_entity*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;



		if (entity->dormant())
			continue;

		if (!entity->is_alive())
			continue;

		if (entity == local_pointer)
			continue;

		static auto set_interpolation_flags = [](player_t* e, int flag) {
			const auto var_map = (uintptr_t)e + 36;
			const auto sz_var_map = *(int*)(var_map + 20);

			for (auto index = 0; index < sz_var_map; index++)
				*(uintptr_t*)((*(uintptr_t*)var_map) + index * 12) = flag;
		};

		set_interpolation_flags(entity, 0);



	}
}
