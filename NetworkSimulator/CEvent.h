#pragma once

namespace WDS {
	class CEvent {
		int32_t eventId;
	public:
		CEvent() :eventId(0) {}
		CEvent(int32_t id) :eventId(id) {}
		CEvent(const CEvent& e) :eventId(e.eventId) {}
		~CEvent() {}
		friend bool operator > (CEvent& e1, CEvent& e2) {
			if (e1.eventId > e2.eventId)
				return true;
			return false;
		}
		friend bool operator < (CEvent& e1, CEvent& e2) {
			if (e1.eventId < e2.eventId)
				return true;
			return false;
		}
		friend bool operator == (CEvent& e1, CEvent& e2) {
			if (e1.eventId == e2.eventId)
				return true;
			return false;
		}
		int32_t getId() {
			return eventId;
		}
	};

	class CEventQ {
		WDLList* q;
	public:
		static CEvent* eventCtr(CEvent* e) {
			CEvent* re;
			re = new CEvent(*e);
			return re;
		}
		static void eventDtr(CEvent* e) {
			delete e;
			return;
		}
		static int32_t eventCmp(CEvent* e1, CEvent* e2) {
			if (e1 > e2)
				return 1;
			else if (e1 < e2)
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