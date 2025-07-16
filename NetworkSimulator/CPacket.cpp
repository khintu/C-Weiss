#include <des_sim_defs.h>

using namespace WDS;

void CPacket::execute() {
	cout << "Executing packet event with Ts: " << getTimestamp() << endl;
	return;
}