#include "../features.hpp"
#include "../../helpers/helpers.h"
NetData g_netdata {};;

void NetData::store( ) {
    int          tickbase;
    StoredData_t* data;

    if ( !interfaces::engine->is_in_game ( ) )
        return;

    if ( !interfaces::engine->is_connected ( ) )
        return;

    local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

    if ( !local_player::m_data.pointer )
        return;

    if ( !local_player::m_data.pointer->is_alive ( ) )
        return;

    tickbase = local_player::m_data.pointer->get_tick_base( );

    // get current record and store data.
    data = &m_data [ tickbase % 150 ];

    data->m_tickbase = tickbase;
    data->m_punch = local_player::m_data.pointer->aim_punch_angle( );
    data->m_punch_vel = local_player::m_data.pointer->aim_punch_angle_vel( );
    data->m_view_offset = local_player::m_data.pointer->view_offset( );
    data->m_velocity_modifier = local_player::m_data.pointer->m_flVelocityModifier( );
}

void NetData::apply( ) {
    int          tickbase;
    StoredData_t* data;
    vec3_t        punch_delta, punch_vel_delta;
    vec3_t       view_delta;
    float        modifier_delta;

    if ( !interfaces::engine->is_in_game ( ) )
        return;

    if ( !interfaces::engine->is_connected ( ) )
        return;

    local_player::m_data.pointer = reinterpret_cast< player_t * >( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player ( ) ) );

    if ( !local_player::m_data.pointer )
        return;

    if ( !local_player::m_data.pointer->is_alive ( ) )
        return;

    tickbase = local_player::m_data.pointer->get_tick_base( );

    // get current record and validate.
    data = &m_data [ tickbase % 150 ];

    if ( local_player::m_data.pointer->get_tick_base( ) != data->m_tickbase )
        return;

    // get deltas.
    // note - dex;  before, when you stop shooting, punch values would sit around 0.03125 and then goto 0 next update.
    //              with this fix applied, values slowly decay under 0.03125.
    punch_delta = local_player::m_data.pointer->aim_punch_angle( ) - data->m_punch;
    punch_vel_delta = local_player::m_data.pointer->aim_punch_angle_vel( ) - data->m_punch_vel;
    view_delta = local_player::m_data.pointer->view_offset( ) - data->m_view_offset;
    modifier_delta = local_player::m_data.pointer->m_flVelocityModifier( ) - data->m_velocity_modifier;

    // set data.
    if ( std::abs( punch_delta.x ) < 0.03125f &&
        std::abs( punch_delta.y ) < 0.03125f &&
        std::abs( punch_delta.z ) < 0.03125f )
        local_player::m_data.pointer->aim_punch_angle( ) = data->m_punch;

    if ( std::abs( punch_vel_delta.x ) < 0.03125f &&
        std::abs( punch_vel_delta.y ) < 0.03125f &&
        std::abs( punch_vel_delta.z ) < 0.03125f )
        local_player::m_data.pointer->aim_punch_angle_vel( ) = data->m_punch_vel;

    if ( std::abs( view_delta.x ) < 0.03125f &&
        std::abs( view_delta.y ) < 0.03125f &&
        std::abs( view_delta.z ) < 0.03125f )
        local_player::m_data.pointer->view_offset( ) = data->m_view_offset;

    if ( std::abs( modifier_delta ) < 0.03125f )
        local_player::m_data.pointer->m_flVelocityModifier( ) = data->m_velocity_modifier;
}

void NetData::reset( ) {
    m_data.fill( StoredData_t( ) );
}