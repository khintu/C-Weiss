#pragma once

namespace WDS {

	class CPacket : public CEvent {
		struct WPacket packet;
	public:
		CPacket():CEvent(), packet() { packet.src = 0x0; packet.dst = 0x0; }
		CPacket(uint32_t ts, uint32_t src, uint32_t dst) :CEvent(ts) \
						{ packet.src = src; packet.dst = dst; }
		CPacket(const CPacket& e):CEvent(e), packet(e.packet) {}
		~CPacket() {}
		
		virtual CEvent* clone() {
			CPacket* newPacket = new CPacket(*this);
			return newPacket;
		}

		virtual void execute();
	};
}