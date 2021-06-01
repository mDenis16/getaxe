#pragma once

class c_engine_prediction {
public:
private:
	 vec3_t unpredicted_eye;
	 vec3_t unpredicted_velocity;
private: 
	void run_think ( );
	player_t * player = nullptr;
public:
	void start ( player_t * _player, c_usercmd * cmd );
	void end ( );
	void sig_scan ( );
private:
	float old_cur_time = 0.f;
	float old_frame_time = 0.f;
	void * move_data = nullptr;
	int * prediction_random_seed = 0;
	int * prediction_player = 0;

public:
	 c_engine_prediction ( );
	 ~c_engine_prediction ( );

};



extern c_engine_prediction * engine_prediction;