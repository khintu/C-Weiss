#pragma once

namespace WDS {

	class CPacket : public CEvent {
		
	public:
		CPacket():CEvent() {}
		CPacket(uint32_t ts):CEvent(ts) {}
		CPacket(const CPacket& e):CEvent(e) {}
		~CPacket() {}
		
		virtual CEvent* clone() {
			CPacket* newPacket = new CPacket(*this);
			return newPacket;
		}

		virtual void execute();
	};
}