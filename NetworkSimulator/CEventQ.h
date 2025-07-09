#pragma once

namespace WDS {

	class CEventQ {
		WDLList* q;
	public:
		static CEvent* eventCtr(CEvent* e) {
			CEvent* re;
			re = e->clone();
			return re;
		}
		static void eventDtr(CEvent* e) {
			delete e;
			return;
		}
		static int32_t eventCmp(CEvent* e1, CEvent* e2) {
			if (*e1 > *e2)
				return 1;
			else if (*e1 < *e2)
				return -1;
			else
				return 0;
		}
		CEventQ() {
			q = WCreateDList((WCMPFP)eventCmp, (WCTRFP)eventCtr, (WDTRFP)eventDtr);
		}
		~CEventQ() {
			WDeleteDList(q);
		}
		bool enQueue(CEvent& e);
		CEvent& deQueue();
		void sort();
	};
}
