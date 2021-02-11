#pragma once

#pragma pack(push, 1)


using CreateClientClassFn = i_client_networkable * ( * )( int, int );
using CreateEventFn = i_client_networkable * ( * )( );

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	const char* m_pNetworkName;
	recv_table* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};


class event_info {
public:
	uint16_t classID; //0x0000 0 implies not in use
	char pad_0002 [ 2 ]; //0x0002 
    float flDelay;
    uint8_t uchPad1[48];
	event_info*pNext;
}; //Size: 0x0044

class i_client_state {
public:
	void force_full_update( )
	{
		m_delta_tick = -1;
	}

	char pad_0000 [ 156 ];
	i_net_channel* m_net_channel;
	uint32_t m_in_challenge_nr;
	char pad_00A4 [ 100 ];
	uint32_t m_signon_state;
	char pad_010C [ 8 ];
	float m_flNextCmdTime;
	uint32_t m_server_count;
	uint32_t m_current_sequence;

	// de aici 
	//char pad_0120 [ 84 ];
	char _0x0120 [ 4 ];
	__int32 m_iClockDriftMgr; //0x0124 
	char _0x0128 [ 68 ];
	__int32 m_iServerTick; //0x016C 
	__int32 m_iClientTick; //0x0170 

	//pana aici
	int m_delta_tick;
	bool m_paused;
	char pad_0179 [ 7 ];
	uint32_t m_view_entity;
	uint32_t m_player_slot;
	char m_levelname [ 260 ];
	char m_level_name_short [ 80 ];
	char m_group_name [ 80 ];
	char pad_032С [ 92 ];
	uint32_t m_max_clients;
	char pad_0314 [ 18820 ];
	float m_last_server_tick_time;
	bool insimulation;
	char pad_4C9D [ 11 ];
	uint32_t oldtickcount;
	float m_tick_remainder;
	float m_frameTime;
	uint32_t m_last_outgoing_command;
	uint32_t m_choked_commands;
	uint32_t m_last_command_ack;
	uint32_t command_ack;
	uint32_t m_sound_sequence;
	char pad_4CC0 [ 80 ];
	vec3_t viewangles;
	char pad_4D1C [ 208 ];
	event_info* m_Events;
};
/*class i_client_state {
	class CClockDriftManager {
	public:
		float clock_offsets [ 17 ];
		uint32_t cur_clock_offset;
		uint32_t m_server_tick;
		uint32_t m_client_tick;
	};
public:
	char pad_0000 [ 0x9C ];
	i_net_channel* m_net_channel;
	uint32_t m_nChallengeNr;
	char pad_00A4 [ 0x64 ];
	uint32_t m_nSignonState;
	char pad_010C [ 0x8 ];
	float m_flNextCmdTime;
	uint32_t m_nServerCount;
	uint32_t m_current_sequence;
	char pad_0120 [ 4 ];
	CClockDriftManager clock_drift_mgr;
	int32_t m_delta_tick;
	bool m_bPaused;
	char pad_0179 [ 7 ];
	uint32_t m_nViewEntity;
	uint32_t m_nPlayerSlot;
	char m_szLevelName [ 260 ];
	char m_szLevelNameShort [ 80 ];
	char m_szGroupName [ 80 ];
	char pad_032C [ 92 ];
	uint32_t m_nMaxClients;
	char pad_0314 [ 18820 ];
	float m_flLastServerTickTime;
	bool in_simulation;
	char pad_4C9D [ 3 ];
	uint32_t oldtickcount;
	float m_tickRemainder;
	float m_frameTime;
	uint32_t m_last_outgoing_command;
	uint32_t m_choked_commands;
	uint32_t m_last_command_ack;
	uint32_t m_command_ack;
	uint32_t m_nSoundSequence;
	char pad_4CC0 [ 80 ];
	vec3_t m_viewangles;
	char pad_4D14 [ 0xD0 ];
	event_info* m_Events;           //0x4DEC
};*/

#pragma pack(pop)