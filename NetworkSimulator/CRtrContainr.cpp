#include <des_sim_defs.h>

using namespace WDS;

void CRtrContainr::populateItr(struct Router* Rtr, struct WRBTree* obj)
{
	CRouter* r;
	r = new CRouter(Rtr, RTR_HOME_IP_ADDR_MASK | Rtr->Id);
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
	Router srch = { rtrId };
	rtr = (struct Router*)WFindInList(gDataRepo->getInetList(), &srch);
	CRouter srch1(rtr, ip);
	result = (CRouter*)WSearchKeyRBT(routerList, &srch1);
	return result;
}