#include <des_sim_defs.h>

using namespace WDS;

void CPktFwd::execute()
{
	rtr->handleEvent(this);

	return;
}

