#include "../features.hpp"
tickbase_system::shift_data tickbase_system::m_shift_data;
tickbase_system::prediction tickbase_system::m_prediction;
void tickbase_system::write_user_cmd ( bf_write * buf, c_usercmd * incmd, c_usercmd * outcmd ) {
	// Generic WriteUserCmd code as seen in every hack ever
	using WriteUsercmd_t = void ( __fastcall * )( void *, c_usercmd *, c_usercmd * );
	static WriteUsercmd_t fnWriteUserCmd = reinterpret_cast< WriteUsercmd_t >( utilities::pattern_scan ( "client.dll", ( "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D" ) ) );

	__asm {
		mov     ecx, buf
		mov     edx, incmd
		push    outcmd
		call    fnWriteUserCmd
		add     esp, 4
	}
}
