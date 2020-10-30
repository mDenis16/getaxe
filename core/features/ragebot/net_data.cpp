#include "../features.hpp"
NetData g_netdata {};;

void NetData::store( ) {
    int          tickbase;
    StoredData_t* data;

    if ( !csgo::local_player || csgo::local_player->is_alive( ) ) {
        return;
    }

    tickbase = csgo::local_player->get_tick_base( );

    // get current record and store data.
    data = &m_data [ tickbase % 150 ];

    data->m_tickbase = tickbase;
    data->m_punch = csgo::local_player->aim_punch_angle( );
    data->m_punch_vel = csgo::local_player->aim_punch_angle_vel( );
    data->m_view_offset = csgo::local_player->view_offset( );
    data->m_velocity_modifier = csgo::local_player->m_flVelocityModifier( );
}

void NetData::apply( ) {
    int          tickbase;
    StoredData_t* data;
    vec3_t        punch_delta, punch_vel_delta;
    vec3_t       view_delta;
    float        modifier_delta;

    if ( !csgo::local_player || csgo::local_player->is_alive()) {
        return;
    }

    tickbase = csgo::local_player->get_tick_base( );

    // get current record and validate.
    data = &m_data [ tickbase % 150 ];

    if ( csgo::local_player->get_tick_base( ) != data->m_tickbase )
        return;

    // get deltas.
    // note - dex;  before, when you stop shooting, punch values would sit around 0.03125 and then goto 0 next update.
    //              with this fix applied, values slowly decay under 0.03125.
    punch_delta = csgo::local_player->aim_punch_angle( ) - data->m_punch;
    punch_vel_delta = csgo::local_player->aim_punch_angle_vel( ) - data->m_punch_vel;
    view_delta = csgo::local_player->view_offset( ) - data->m_view_offset;
    modifier_delta = csgo::local_player->m_flVelocityModifier( ) - data->m_velocity_modifier;

    // set data.
    if ( std::abs( punch_delta.x ) < 0.03125f &&
        std::abs( punch_delta.y ) < 0.03125f &&
        std::abs( punch_delta.z ) < 0.03125f )
        csgo::local_player->aim_punch_angle( ) = data->m_punch;

    if ( std::abs( punch_vel_delta.x ) < 0.03125f &&
        std::abs( punch_vel_delta.y ) < 0.03125f &&
        std::abs( punch_vel_delta.z ) < 0.03125f )
        csgo::local_player->aim_punch_angle_vel( ) = data->m_punch_vel;

    if ( std::abs( view_delta.x ) < 0.03125f &&
        std::abs( view_delta.y ) < 0.03125f &&
        std::abs( view_delta.z ) < 0.03125f )
        csgo::local_player->view_offset( ) = data->m_view_offset;

    if ( std::abs( modifier_delta ) < 0.03125f )
        csgo::local_player->m_flVelocityModifier( ) = data->m_velocity_modifier;
}

void NetData::reset( ) {
    m_data.fill( StoredData_t( ) );
}