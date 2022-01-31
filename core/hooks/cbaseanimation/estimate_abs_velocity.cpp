#include "../hooks.hpp"
#include "../../features/animations/animations_manager.h"


namespace hooks::callback {
    void __fastcall estimate_abs_velocity(player_t* player, void* edx, vec3_t& velocity) {

        if (hooks::unloading)
            return estimate_abs_velocity_original(player, edx, velocity);

        if (!player)
            return estimate_abs_velocity_original(player, edx, velocity);

        if (player && player->index() <= 64)
        {
            const auto& anim = anim_manager.allow_animation[player->index()];
            if (anim) {

               // std::cout << " fixed estimated velocity " << std::endl;
                velocity = anim_manager.velocities[player->index()];

            	return;

            }
            
        }

        if (player && player->simulation_time() < player->get_old_simulation_time())
            return;


         estimate_abs_velocity_original(player, edx, velocity);
    }
}