#include "../../features/features.hpp"
#include "../hooks.hpp"



namespace hooks::callback {
    bool __fastcall should_interpolate(player_t* player) {

        if (hooks::unloading)
            return should_interpolate_original(player);

        if (!player)
            return should_interpolate_original(player);

        bool result = should_interpolate_original(player);

        if (player == local_pointer) {
        	return false;
        }

    

        return  should_interpolate_original(player);
    }
}