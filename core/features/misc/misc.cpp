#include "../../helpers/helpers.h"
#include "misc.h"

void c_misc::run_createmove(c_usercmd* cmd)
{
	bunnyhop(cmd);
	autostrafe(cmd);
}
