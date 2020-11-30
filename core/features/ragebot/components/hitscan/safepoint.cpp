#include "../../../features.hpp"
#include "../../../../helpers/helpers.h"


namespace aimbot {

	bool does_point_intersect ( target & entity, vec3_t point, int hitbox, matrix_t bones [ 128 ] ) {

		auto _hitbox = entity.hitbox_set->hitbox ( hitbox );

		const auto mod = _hitbox->radius != -1.f ? _hitbox->radius : 0.f;
		vec3_t max = math::vector_transform ( _hitbox->maxs, bones [ _hitbox->bone ] );
		vec3_t min = math::vector_transform ( _hitbox->mins, bones [ _hitbox->bone ] );

		if ( auto intersection = get_intersect_point ( engine_prediction::unpredicted_eye, point, min, max, _hitbox->radius ); intersection )
			return true;
			
		return false;
	}
}