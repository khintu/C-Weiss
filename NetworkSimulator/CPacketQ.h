#pragma once

namespace WDS {

	struct WPacket
	{
		uint32_t src; // source IP
		uint32_t dst; // destination IP
		uint8_t data[1496]; // MTU 1500 = 4+4+1496
	};

	struct WPktQNode
	{
		struct WPacket pkt;
		struct WPktQNode* next;
	};

	class CPacketQ
	{
		struct WPktQNode* head, * tail;
		uint32_t size; // Current size of the packet queue
		uint32_t maxSize; // Maximum size of the packet queue
	public:
		CPacketQ(uint32_t maxSize = 100) :maxSize(maxSize), head(NULL), tail(NULL), size(0) {}
		~CPacketQ();

		struct WPacket* deque();
		void enque(struct WPacket& x);
	};
}