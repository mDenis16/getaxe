#include "../../features.hpp"

#define scale(a, b)  hitbox->radius * (a / b )


c_multipoint c_target::populate_multipoint(const int& hit_box, c_lag_record* record, bool should_disable)
{
	auto model = this->model();

	if (!model)
		return c_multipoint();

	auto studio_model = interfaces::model_info->get_studio_model(model);
	if (!studio_model)
		return c_multipoint();

	auto hitbox_set = studio_model->hitbox_set(0);
	if (!hitbox_set)
		return c_multipoint();


	c_multipoint mp;

	studio_box_t* hitbox = hitbox_set->hitbox(hit_box);

	const auto mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;

	
	auto maxs = math::vector_transform(hitbox->maxs + mod, record->bones[hitbox->bone]);
	auto mins = math::vector_transform(hitbox->mins - mod, record->bones[hitbox->bone]);

	mp.center = (mins + maxs) * 0.5f;

	c_point point;
	point.pos = mp.center;
	point.safe = false;
	point.damage = 0;


	vec3_t current_angles = math::calc_angle(mp.center, local_player::data().eye_position);

	vec3_t forward = math::angle_vector(current_angles);

	vec3_t right = forward.cross(vec3_t(0, 0, 0.95f));
	vec3_t left = vec3_t(-right.x, -right.y, right.z);

	vec3_t top = vec3_t(0, 0, 0.95f);
	vec3_t bot = vec3_t(0, 0, -0.95f);


	if (hit_box == hitbox_head) {

		mp.points.resize(7);
		std::fill(mp.points.begin(), mp.points.end(), point);

		vec3_t middle = (top - bot);
		mp.points[1].pos.z += middle.z;

		mp.points[2].pos += vec3_t(right.x * scale(ragebot->cfg->head_scale, 100.f), right.y * scale(ragebot->cfg->head_scale, 100.f), top.z * hitbox->radius * 0.95f);
		mp.points[3].pos += vec3_t(left.x * scale(ragebot->cfg->head_scale, 100.f), left.y * scale(ragebot->cfg->head_scale, 100.f), top.z * hitbox->radius * 0.95f);
		mp.points[4].pos += vec3_t(right.x * scale(ragebot->cfg->head_scale, 100.f), right.y * scale(ragebot->cfg->head_scale, 100.f), middle.z);
		mp.points[5].pos += vec3_t(left.x * scale(ragebot->cfg->head_scale, 100.f), left.y * scale(ragebot->cfg->head_scale, 100.f), middle.z);
		mp.points[6].pos = vec3_t(mp.center.x, mp.center.y, mp.center.z + top.z * hitbox->radius * (this->is_moving() ? 0.65f : 0.99f));

	}
	else if (hit_box >= hitbox_pelvis && hit_box <= hitbox_upper_chest) {

		mp.points.resize(3);
		std::fill(mp.points.begin(), mp.points.end(), point);

		mp.points[1].pos += right * (hitbox->radius * (ragebot->cfg->point_scale / 100.f));
		mp.points[2].pos += left * (hitbox->radius * (ragebot->cfg->point_scale / 100.f));

	}
	else
	{
		mp.points.resize(1);
		std::fill(mp.points.begin(), mp.points.end(), point);

		mp.points[0].pos = mp.center;
	}


	return mp;
}
