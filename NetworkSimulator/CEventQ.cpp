#include <des_sim_defs.h>

using namespace WDS;

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
	l->DTOR(p->data);
	free(p);
	l->count--;
	return re;
}

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