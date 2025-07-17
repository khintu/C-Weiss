#include <des_sim_defs.h>

using namespace WDS;

void CPacket::execute() {
	CRouter* rtr;

	cout << "Executing packet event with Ts: " << getTimestamp() << endl;
	try {
		rtr = gDataRepo->getRouterContainer()->getRtrByIp(packet.src);
		rtr->postPacket(packet);
	}catch (Exception& e) {
		e.printErr();
		cerr << "Packet dropped due to error in router!" << endl;
	}
	return;
}