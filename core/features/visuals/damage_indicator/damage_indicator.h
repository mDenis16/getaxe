#pragma once

class damage_log {
public: 
	int dmg;
	float time;
	vec3_t position;
	damage_log ( const int & _dmg, const vec3_t & _pos );
	~damage_log ( );

};

class damage_indicator {
private:

	std::vector< damage_log *> logs;

public:
	damage_indicator ( ) {
		logs.reserve ( 100 );
	}

	void addDmg ( damage_log * log );

	void on_render ( );

};


extern damage_indicator * dmg_indicator;