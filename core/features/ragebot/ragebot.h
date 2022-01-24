#pragma once
#include <vector>

struct c_point {
	vec3_t pos = vec3_t();
	int damage;
	bool safe = false;
};

struct c_multipoint {
	std::vector<c_point> points = {};
	vec3_t center = vec3_t();
};
struct c_collision {
	vec3_t mins = vec3_t(), maxs = vec3_t();
	float radius = 0.f;
};
struct c_bestpoint {
	vec3_t position = vec3_t();
	vec3_t center = vec3_t();
	int dmg = 0;
	int hitbox = 0;
	c_collision col = {};
	int bone = 0;
	bool should_trace = false;
	bool finished_trace = false;
	bool passed_hitchance = false;
};

class c_target : public player_t
{
public:

	c_multipoint scanned_points;

	c_multipoint populate_multipoint(const int& hit_box, c_lag_record* record, bool should_disable);

	c_bestpoint best_point(c_lag_record* record);

	c_lag_record* select_best_record();
};


class c_ragebot
{
public:
	std::vector<c_target*> possible_targets;

	c_target* best_target;

	rage_weapon_settings* cfg = nullptr;


	void populate_target_list();
	void recalculate_list();



	std::vector<int> hitscan_list;


	void run(c_usercmd* cmd);

};
extern c_ragebot* ragebot;

