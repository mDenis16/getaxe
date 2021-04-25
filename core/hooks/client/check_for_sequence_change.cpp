#include "../../features/features.hpp"
#include "../hooks.hpp"


namespace hooks::callback {
	void __fastcall check_for_sequence_change ( uintptr_t * ecx, uintptr_t * edx, uintptr_t * hdr, int cur_sequence, bool force_new_sequence, bool ) {

		return check_for_sequence_change_original ( ecx, edx, hdr, cur_sequence, force_new_sequence, false );

	}
}