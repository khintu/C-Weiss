#include <des_sim_defs.h>

using namespace WDS;

void CRouter::handleEvent(CEvent* e) 
{
	
	return;
}

void CRouter::postPacket(struct WPacket& pkt) {
	char buffer[16];
	if (temp >= 100.0) { /* Simulate router going down due to high temperature */
		isUp = false;
		throw Exception( "Router " + name + " is down due to high temperature: " \
										+ string(itoa(temp, buffer, 10)));
		return;
	}
	pQ.enque(pkt);
	temp += 0.1; /* Simulate temperature increase */
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