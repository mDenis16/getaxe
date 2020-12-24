#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
	std::vector<int> hitscan_list;
	void populate_hitscan ( ) {
		hitscan_list.clear ( );
		int target_hitbox = config.ragebot_prioritize_hitbox == 0 ? hitbox_head : hitbox_chest;
		hitscan_list.push_back ( target_hitbox );

		if ( config.ragebot_head_scan ) {
			hitscan_list.push_back ( (int)hitboxes::hitbox_head );
		}

		if ( config.ragebot_body_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_upper_chest );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_chest );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_pelvis );
		}

		if ( config.ragebot_feet_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_foot );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_foot );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_thigh );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_thigh );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_calf );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_calf );
		}
		if ( config.ragebot_arms_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_upper_arm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_upper_arm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_forearm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_forearm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_hand );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_hand );
		}


		for ( size_t i = hitscan_list.size ( ) - 1; i >= 0; i-- ) {
			if ( hitscan_list.at ( i ) == target_hitbox ) {
				hitscan_list.erase ( hitscan_list.begin ( ) + i );
				break;
			}
		}
	}
 
}

