#pragma once

namespace WDS {
	class CRtrContainr
	{
		struct WRBTree* routerList;
	public:
		CRtrContainr() {
			routerList = WCreateRBT((WCMPFP)RouterCmp, (WCTRFP)RouterCtr, (WDTRFP)RouterDtr);
		}
		~CRtrContainr() {
			WDeleteRBT(routerList);
		}
		static int32_t RouterCmp(CRouter* r1, CRouter* r2) {
			return (r1->getId() > r2->getId()) ? 1 : 
								(r1->getId() < r2->getId()) ? -1 : 0;
		}
		static CRouter* RouterCtr(CRouter* r) {
			return r;
		}
		static void RouterDtr(CRouter* r) {
			delete r;
		}
		static void populateItr(struct Router* Rtr, struct WRBTree* obj);
		void populateObj(WLList* inetList);

	};
}
