#include "../../features.hpp"

void c_ragebot::recalculate_list() {


	if (ragebot->cfg->hitscan[HITSCAN::HITSCAN_HEAD]) {
		hitscan_list.push_back((int)hitboxes::hitbox_head);
	}

	if (ragebot->cfg->hitscan[HITSCAN::HITSCAN_BODY]) {
		hitscan_list.push_back((int)hitboxes::hitbox_upper_chest);
		hitscan_list.push_back((int)hitboxes::hitbox_chest);
		hitscan_list.push_back((int)hitboxes::hitbox_pelvis);
	}

	if (ragebot->cfg->hitscan[HITSCAN::HITSCAN_FEETS]) {
		hitscan_list.push_back((int)hitboxes::hitbox_left_foot);
		hitscan_list.push_back((int)hitboxes::hitbox_right_foot);
		hitscan_list.push_back((int)hitboxes::hitbox_right_thigh);
		hitscan_list.push_back((int)hitboxes::hitbox_left_thigh);
		hitscan_list.push_back((int)hitboxes::hitbox_left_calf);
		hitscan_list.push_back((int)hitboxes::hitbox_right_calf);
	}
	if (ragebot->cfg->hitscan[HITSCAN::HITSCAN_ARMS]) {
		hitscan_list.push_back((int)hitboxes::hitbox_right_upper_arm);
		hitscan_list.push_back((int)hitboxes::hitbox_left_upper_arm);
		hitscan_list.push_back((int)hitboxes::hitbox_left_forearm);
		hitscan_list.push_back((int)hitboxes::hitbox_right_forearm);
		hitscan_list.push_back((int)hitboxes::hitbox_left_hand);
		hitscan_list.push_back((int)hitboxes::hitbox_right_hand);
	}


	for (size_t i = hitscan_list.size() - 1; i >= 0; i--) {
		if (hitscan_list.at(i) == ragebot->cfg->prority_hitbox) {
			hitscan_list.erase(hitscan_list.begin() + i);
			break;
		}
	}
}
