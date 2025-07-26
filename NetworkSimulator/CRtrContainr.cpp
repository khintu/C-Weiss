#include <des_sim_defs.h>

using namespace WDS;

void CRtrContainr::populateItr(struct Router* Rtr, struct WRBTree* obj)
{
	CRouter* r;
	r = new CRouter(Rtr, RTR_HOME_IP_ADDR_MASK | Rtr->Id);
	Rtr->super = r;
	WInsertKeyRBT(obj, r);
	return;
}

void CRtrContainr::populateObj(WLList* inetList)
{
	WIteratorList3(inetList, (void*)routerList, (void (*)(void*, void*))populateItr);

	return;
}

CRouter* CRtrContainr::getRtrByIp(uint32_t ip)
{
	uint32_t rtrId;
	CRouter* result;
	struct Router* rtr;

	rtrId = RTR_HOME_IP_ADDR_MASK ^ ip;
	struct Router srch1 = { rtrId };
	rtr = (struct Router*)WFindInList(gDataRepo->getInetList(), &srch1);
	if (rtr == NULL)
		throw Exception("Router not found in container by IP address: " + string(decimal2dotted32(ip)));
	CRouter srch2(rtr, ip);
	result = (CRouter*)WSearchKeyRBT(routerList, &srch2);
	return result;
}

void CRtrContainr::coolDownItr(CRouter* r) {
	r->serviceCoolDown();
	return;
}

void CRtrContainr::processCoolDown()
{
	WIteratorRBT(routerList, (void (*)(void*)) coolDownItr);

	return;
}