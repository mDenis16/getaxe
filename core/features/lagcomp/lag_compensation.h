#pragma once
#include <utility>
#include <deque>
#include <vector>

class c_lag_record
{
public:
	float simulation_time = 0.f;
	matrix3x4a_t bones[128];
};

class lag_entity : public player_t
{
public:

	void update_animations();
	void build_bones();

	void did_network_update();


	std::vector<c_lag_record>& records ()
	{
		static std::vector<c_lag_record> _internal_records; static bool first_time = false;
		if (!first_time) {
			_internal_records.reserve(32);
			first_time = false;
		}
		return _internal_records;
	}

	/// <summary>
	/// Called on entity handler delete
	/// </summary>
	void on_destruct()
	{
		
	}

}; 

class lag_compensation
{
public:

	

	void frame_update_post_entity_think();
	
};
