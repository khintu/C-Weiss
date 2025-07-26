#include <des_sim_defs.h>

using namespace WDS;

void CRouter::handleEvent(CEvent* e) 
{
	struct WPacket* pkt;
	struct RouteEntry* nxtHop;
	struct Router* dstRtr;

	try {
		pkt = pQ.deque();

		if (homeIp == pkt->dst) {
			/* Packet is destined for this router */
			cout << "Packet [" << hex << pkt->src << ":" << \
				hex << pkt->dst << "] received at destination router : " << getId() << endl;
			delete pkt; // No further processing needed
			return;
		}

		if ((nxtHop = longestPrefixMatch(r->FwdgTbl, pkt->dst)) == NULL)
			throw Exception("No route found for packet destination: " + string(decimal2dotted32(pkt->dst)));

		dstRtr = gIntfTbl[nxtHop->I];
		static_cast<CRouter*>(dstRtr->super)->postPacket(*pkt);
		
		CPktFwd newEvt(e->getTimestamp() + 1, static_cast<CRouter*>(dstRtr->super));
		gDataRepo->getEventQueue()->enQueue(newEvt);
		
		delete pkt;
	}
	catch (Exception& e) {
		e.printErr();
		cerr << "Could not handle event pkt fwd for router: " << getId() << endl;
	}
	return;
}

void CRouter::postPacket(struct WPacket& pkt) {
	char buffer[16];
	if (temp >= 100.0) { /* Simulate router going down due to high temperature */
		isUp = false;
		throw Exception( "Router " + name + " is down due to over heating: " \
										+ string(_itoa((int)temp, buffer, 10)));
		return;
	}
	pQ.enque(pkt);
	temp += (float)0.1; /* Simulate temperature increase */
	return;
}

void CRouter::serviceCoolDown()
{
	if (isUp == false) {
		temp -= 1.0;
		if (temp <= 0.0) {
			temp = 0.0;
			isUp = true;
		}
	}
	return;
}