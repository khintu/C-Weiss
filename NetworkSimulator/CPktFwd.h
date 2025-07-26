#pragma once

namespace WDS {

	class CPktFwd : public CEvent {
		CRouter* rtr;

	public:
		CPktFwd() : CEvent(), rtr(nullptr) {}
		CPktFwd(uint32_t ts, CRouter* r) : CEvent(ts), rtr(r) {}
		CPktFwd(const CPktFwd& e) : CEvent(e), rtr(e.rtr) {}
		~CPktFwd() {}

		virtual CEvent* clone() {
			CPktFwd* newEvt = new CPktFwd(*this);
			return newEvt;
		}

		virtual void execute();
	};
}