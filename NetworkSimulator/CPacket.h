#pragma once

namespace WDS {

	class CPacket : public CEvent {
		
	public:
		CPacket():CEvent() {}
		CPacket(int32_t id):CEvent(id) {}
		CPacket(const CPacket& e):CEvent(e) {}
		~CPacket() {}
		
		virtual CEvent* clone() {
			CPacket* newPacket = new CPacket(*this);
			return newPacket;
		}

		virtual void execute() {
			cout << "Executing packet event with ID: " << getId() << endl;
		}
	};
}