#pragma once

#define max_buffer_size 1024
#include <stdio.h>
#include <cstdarg>

class i_hud_chat
{
public:
	enum chat_filters
	{
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		//=============================================================================
		// HPE_BEGIN:
		// [tj]Added a filter for achievement announce
		//=============================================================================

		CHAT_FILTER_ACHIEVEMENT = 0x000020,

		//=============================================================================
		// HPE_END
		//=============================================================================
	};

	void chat_print( int player_index, int filter, const char* format, ... ) {
		static char buf[ max_buffer_size ] = "";
		va_list va;
		va_start( va, format );
		vsnprintf_s( buf, max_buffer_size, format, va );
		va_end( va );

		using original_fn = void( __cdecl* )( void*, int, int, const char*, ... );
		( *( original_fn** ) this )[ 27 ]( this, player_index, filter, buf );
	}
};