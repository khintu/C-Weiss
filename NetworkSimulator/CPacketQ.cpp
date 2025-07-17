#include <des_sim_defs.h>

using namespace WDS;

struct WPacket* CPacketQ::deque()
{
	WPacket* pkt;
	WPktQNode* tmp;

	if (head == NULL)
		throw WDS::Exception("Packet queue empty");

	pkt = new WPacket;
	memcpy(pkt, &head->pkt, sizeof(struct WPacket));
	
	tmp = head;
	head = head->next;
	delete tmp;
	size--;

	if (head == NULL)
		tail = NULL;
	return pkt;
}

void CPacketQ::enque(struct WPacket& x)
{
	WPktQNode* node;

	if (size >= maxSize)
		throw Exception("No more space in packet queue");
	
	node = new WPktQNode();
	memcpy(&node->pkt, &x, sizeof(struct WPacket));
	node->next = NULL;
	if (tail) {
		tail->next = node;
		tail = node;
	}
	if (!head && !tail) {
		tail = head = node;
	}
	size++;
	return;
}

CPacketQ::~CPacketQ()
{
	WPktQNode* i, *tmp;
	for (i = head; i != NULL; ) {
		tmp = i;
		i = i->next;
		delete tmp;
	}
	return;
}