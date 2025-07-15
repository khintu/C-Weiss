#include <des_sim_defs.h>

using namespace WDS;

void CRtrContainr::populateItr(struct Router* Rtr, struct WRBTree* obj)
{
	CRouter* r;
	r = new CRouter(Rtr);
	WInsertKeyRBT(obj, r);
	return;
}

void CRtrContainr::populateObj(WLList* inetList)
{
	WIteratorList3(inetList, (void*)routerList, (void (*)(void*, void*))populateItr);

	return;
}