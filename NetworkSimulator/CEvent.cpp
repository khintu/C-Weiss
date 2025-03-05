#include <des_sim_defs.h>
#include <CEvent.h>

extern "C" {
	void* WExtractHeadFrmDList(struct WDLList* l)
	{
		struct DLNode* p;
		void* re;
		if (l->head == NULL)
			return NULL;
		p = l->head;
		re = l->CTOR(p->data);
		if (p->next != NULL)
			p->next->prev = NULL;
		l->head = p->next;
		if (l->tail == p)
			l->tail = NULL;
		free(p);
		l->count--;
		return re;
	}
}

using namespace WDS;

bool CEventQ::enQueue(CEvent& e)
{
	WAppendToDList(q, &e);
	return true;
}

CEvent& CEventQ::deQueue()
{
	CEvent* re;
	re = (CEvent*)WExtractHeadFrmDList(q);
	if (re == NULL)
		throw WDS::Exception("Queue is empty");
	return *re;
}

void CEventQ::sort()
{
	WQuickSortDList(q);
	return;
}