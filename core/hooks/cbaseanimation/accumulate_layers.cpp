#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
    void __fastcall accumulate_layers ( uintptr_t * player, uintptr_t edx, void * setup, vec3_t * pos, float time, uintptr_t * q ) {
      
             return accumulate_layers_original (  player, edx, setup, pos, time, q );
       
        
        /*if ( !player->is_valid ( ) )
            return ORIGINAL_FAST ( accumulate_layers, player, setup, pos, time, q );

        std::array<size_t, 13> layers;
        layers.fill ( layers.size ( ) );

        for ( auto i = 0u; i < player->get_anim_layers ( )->size ( ); i++ ) {
            const auto & layer = ( *player->get_anim_layers ( ) ) [ i ];

            if ( layer.weight > 0 && layer.order >= 0 && layer.order < layers.size ( ) )
                layers [ layer.order ] = i;
        }

        for ( auto i = 0u; i < layers.size ( ); i++ ) {
            const auto layer_num = layers [ i ];

            if ( layer_num < 0 || layer_num >= player->get_anim_layers ( )->size ( ) )
                continue;

            const auto & layer = ( *player->get_anim_layers ( ) ) [ layer_num ];

            if ( layer.weight <= 1 )
                continue;

            ( *setup )->accumulate_pose ( pos, q, layer.sequence, layer.cycle, layer.weight, global_vars_base->curtime, player->get_ik_context ( ) );
        }*/
    }
}