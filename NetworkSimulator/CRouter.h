#pragma once

namespace WDS {

	class CRouter : public CEntity {
		struct Router* r; /* Forwarding table reference only */
		uint32_t MTU; /* Maximum Transmission Unit */
		bool isUp; /* Router is up or down */
		float temp; /* Router temperature to calculate Up or Down state */
		CPacketQ pQ; /* Buffer for congestion control, raw packets from events */
		string name; /* Router (Geographic) name for identification */
		uint32_t homeIp; /* Home IP address of the router */
	public:
		CRouter(struct Router* x, uint32_t Ip): r(x), MTU(1500), \
						isUp(true), temp(0.0), pQ(), homeIp(Ip) {}
		~CRouter() {}
		void setName(const string& n) { name = n; }
		uint32_t getId() const { return r->Id; }
		
		virtual void handleEvent(CEvent* e);
		void postPacket(struct WPacket& pkt);
		void serviceCoolDown();
	};
}