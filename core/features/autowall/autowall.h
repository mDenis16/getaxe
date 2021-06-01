#pragma once
namespace autowall {
	struct FireBulletData_t {
		vec3_t			vecPosition;
		vec3_t			vecDir;
		trace_t			enterTrace;
		trace_filter	filter;

		float			flCurrentDamage;
		int				iPenetrateCount;

		bool failed_penetrate = false;
		int iPenetrateCountLimit = 4;
		int penetrationTry = 0;
		player_t * target = nullptr;
	};
	int get_damage ( player_t * from, const vec3_t & start, const vec3_t & end, FireBulletData_t & data );

	void ScaleDamage ( int iHitGroup, player_t * pEntity, float flWeaponArmorRatio, float & flDamage );
	void ClipTraceToPlayers ( const vec3_t & vecAbsStart, const vec3_t & vecAbsEnd, unsigned int fMask, trace_filter * pFilter, trace_t * pTrace );
	bool IsBreakableEntity ( player_t * pEntity );
	bool trace_to_exit ( vec3_t start, vec3_t dir, vec3_t & end, trace_t & trEnter, trace_t & trExit, float flStepSize, float flMaxDistance );
	bool TraceToExit ( trace_t * pEnterTrace, trace_t * pExitTrace, vec3_t vecPosition, vec3_t vecDirection );
	bool HandleBulletPenetration ( player_t * pLocal, surfacedata_t * pEnterSurfaceData, weapon_info_t * pWeaponData, FireBulletData_t & data );
	bool SimulateFireBullet ( player_t * pLocal, weapon_t * pWeapon, FireBulletData_t & data );
	void TraceLine ( const vec3_t & vecAbsStart, const vec3_t & vecAbsEnd, unsigned int fMask, player_t * pSkip, trace_t * pTrace );

	float get_thickness ( vec3_t & start, vec3_t & end );
	int get_peneration_count ( player_t * player, vec3_t from, vec3_t to );
	bool trace_to_exit_short ( vec3_t & point, vec3_t & dir, const float step_size, float max_distance );
	int hbp ( weapon_t * weapon, vec3_t from, vec3_t to );
	bool can_hit_float_point ( const vec3_t & point, const vec3_t & source );
	int get_dmg ( player_t * from, const vec3_t & to );

}