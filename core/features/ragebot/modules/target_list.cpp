#include "../../features.hpp"

void c_ragebot::populate_target_list() {

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		c_target* entity = reinterpret_cast<c_target*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity == local_player::m_data.pointer)
			continue;

		if (!entity->is_alive())
			continue;

		if ( entity->dormant ( ) )
			continue;

		if (entity->is_teammate())
			continue;

		possible_targets.push_back(entity);

	}

	struct {
		bool operator()(struct c_target* a, struct c_target* b) const {
			auto angle_a = math::calc_angle(local_player::m_data.eye_position, a->get_eye_pos()); angle_a.angle_normalize();
			auto angle_b = math::calc_angle(local_player::m_data.eye_position, b->get_eye_pos()); angle_b.angle_normalize();
			auto fov_a = math::get_fov(local_player::m_data.orig_viewangle, angle_a, localdata.eye_position.distance_to(a->origin()));
			auto fov_b = math::get_fov(local_player::m_data.orig_viewangle, angle_b, localdata.eye_position.distance_to(b->origin()));
			return  fov_a < fov_b;

		}
	} target_sort;



	std::sort(possible_targets.begin(), possible_targets.end(), target_sort);
	if (possible_targets.size() > 3)
		possible_targets.erase(possible_targets.begin() + 3, possible_targets.end());
}

