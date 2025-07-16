#pragma once

namespace WDS {

	class CEvent {
		uint32_t eventTime;
	public:
		CEvent() :eventTime(0) {}
		CEvent(uint32_t ts) :eventTime(ts) {}
		CEvent(const CEvent& e) :eventTime(e.eventTime) {}
		virtual ~CEvent() {}
		friend bool operator > (CEvent& e1, CEvent& e2) {
			if (e1.eventTime > e2.eventTime)
				return true;
			return false;
		}
		friend bool operator < (CEvent& e1, CEvent& e2) {
			if (e1.eventTime < e2.eventTime)
				return true;
			return false;
		}
		friend bool operator == (CEvent& e1, CEvent& e2) {
			if (e1.eventTime == e2.eventTime)
				return true;
			return false;
		}
		uint32_t getTimestamp() const {
			return eventTime;
		}
		virtual void execute() = 0;
		virtual CEvent* clone() = 0;
	};
}