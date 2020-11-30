#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"

namespace aimbot {
	std::vector<int> hitscan_list;
	void populate_hitscan ( ) {
		hitscan_list.clear ( );

		hitscan_list.push_back ( variables::ragebot::prioritize_hitbox == 0 ? hitbox_head : hitbox_chest );

		if ( variables::ragebot::head_scan ) {
			hitscan_list.push_back ( (int)hitboxes::hitbox_head );
		}

		if ( variables::ragebot::body_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_upper_chest );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_chest );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_pelvis );
		}

		if ( variables::ragebot::feet_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_foot );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_foot );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_thigh );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_thigh );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_calf );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_calf );
		}
		if ( variables::ragebot::arms_scan ) {
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_upper_arm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_upper_arm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_forearm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_forearm );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_left_hand );
			hitscan_list.push_back ( ( int ) hitboxes::hitbox_right_hand );
		}

		std::sort ( hitscan_list.begin ( ), hitscan_list.end ( ) );
		hitscan_list.erase ( std::unique ( hitscan_list.begin ( ), hitscan_list.end ( ) ), hitscan_list.end ( ) );
	}
 
}

