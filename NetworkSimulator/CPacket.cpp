#include <des_sim_defs.h>

using namespace WDS;

void CPacket::execute() {
	CRouter* rtr;

	cout << "Executing packet event with Ts: " << getTimestamp() << endl;
	rtr = gDataRepo->getRouterContainer()->getRtrByIp(packet.src);
	if (rtr == NULL)
		throw Exception("Packet source router not found in container");
	try {
		rtr->postPacket(packet);
	}catch (Exception& e) {
		e.printErr();
		cerr << "Packet dropped due to error in postPacket" << endl;
	}
	return;
}