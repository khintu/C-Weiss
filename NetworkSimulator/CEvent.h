#pragma once

namespace WDS {

	class CEvent {
		int32_t eventId;
		uint32_t eventTime;
	public:
		CEvent() :eventId(0), eventTime(0) {}
		CEvent(int32_t id) :eventId(id), eventTime(0) {}
		CEvent(const CEvent& e) :eventId(e.eventId), eventTime(e.eventTime) {}
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
		virtual void execute() = 0;
		virtual CEvent* clone() = 0;
	};
}