#include "../../features.hpp"

c_bestpoint c_target::best_point(c_lag_record* record) {



	autowall::FireBulletData_t awall = { };
	awall.target = reinterpret_cast<player_t*>(this);
	
	c_bestpoint data = {};
	bool found_fatal_hit = false;
	bool is_newest_record = record->simulation_time == this->simulation_time();


	auto dist = math::calc_distance(local_pointer->origin(), this->origin(), false);
	float min_dmg = ragebot->cfg->autowall_min_dmg;


	for (auto& hitbox : ragebot->hitscan_list) { /*loop through our hitscan list*/



		auto points = this->populate_multipoint(hitbox, record, false);



		for (auto& point : points.points) {


			if (!point.safe)
				continue;


			int dmg = autowall::get_damage(local_pointer, localdata.eye_position, point.pos, awall);


			found_fatal_hit = (dmg >= this->health() + 10) && hitbox != hitbox_head;


			if ((dmg >= data.dmg && (dmg >= min_dmg)) || found_fatal_hit) {
				data.position = point.pos;
				data.dmg = dmg;
				data.hitbox = hitbox;
				data.center = points.center;

				if (found_fatal_hit) {
					break;
				}
			}

		}
		if (found_fatal_hit)
			break;
	}


	return data;
}