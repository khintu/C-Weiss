#include <des_sim_defs.h>

using namespace WDS;

void CPacket::execute() {
	CRouter* rtr;

	cout << "Executing packet event with Ts: " << getTimestamp() << endl;
	try {
		rtr = gDataRepo->getRouterContainer()->getRtrByIp(packet.src);
		rtr->postPacket(packet);
		CPktFwd newEvt(getTimestamp() + 1, rtr);
		gDataRepo->getEventQueue()->enQueue(newEvt);
	}catch (Exception& e) {
		e.printErr();
		cerr << "Packet dropped due to error in posting to router!" << endl;
	}
	return;
}