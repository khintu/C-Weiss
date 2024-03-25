#include <stdlib.h>
#include <c-algorithm.h>



struct WDLList* WCreateDList(int (*CMP)(const void* x, const void* y), \
														void* (*CTOR)(void* x), \
														void (*DTOR)(void* x))
{
	struct WDLList* p;

	p = (struct WDLList*)calloc(1, sizeof(struct WDLList));
	if (p)
		p->CMP = CMP, p->CTOR = CTOR, p->DTOR = DTOR;
	return p;
}

int WAddToDList(struct WDLList* l, void* data)
{
	struct DLNode* p, * tmp;

	for (p = l->head; p && p->next != NULL; p = p->next)
		;
	if ((tmp = (struct DLNode*)calloc(1, sizeof(struct DLNode))) == NULL)
		return -1;

	if (p == NULL)
		p = tmp, l->head = p;
	else
		p->next = tmp, tmp->prev = p, p = tmp;

	l->tail = p;
	l->count++;
	p->data = (*l->CTOR)(data);
	return 0;
}

void WDeleteDList(struct WDLList* l)
{
	struct DLNode* p;

	for (p = l->head; p ;)
	{
		(*l->DTOR)(p->data);
		if (p->next)
			p = p->next, free(p->prev);
		else
			free(p), p = NULL;
		l->count--;
	}
	free(l);
	return;
}

void* WFindInDList(struct WDLList* l, void* key)
{
	struct DLNode* p;
	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = p->next)
		;
	return p ? p->data : NULL;
}

int WDeleteFromDList(struct WDLList* l, void* key)
{
	struct DLNode* p;

	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = p->next)
		;
	if (p == NULL) /* not found */
		return -2;

	if (p->prev != NULL)
		p->prev->next = p->next;
	if (p->next != NULL)
		p->next->prev = p->prev;
	if (l->head == p)
		l->head = p->next;
	if (l->tail == p)
		l->tail = p->prev;
	(*l->DTOR)(p->data);
	free(p);
	l->count--;
	return 0;
}

int WInsertToDList(struct WDLList* l, void* key, void* data)
{
	struct DLNode* p, * tmp;
	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = p->next)
		;
	if (p == NULL) /* key not found */
		return -2;

	if ((tmp = (struct DLNode*)calloc(1, sizeof(struct DLNode))) == NULL)
		return -1;

	tmp->data = (*l->CTOR)(data);
	if (p->next != NULL)
		p->next->prev = tmp, tmp->next = p->next;
	tmp->prev = p;
	p->next = tmp;
	if (p == l->tail)
		l->tail = tmp;
	l->count++;
	return 0;
}

int WPrependToDList(struct WDLList* l, void* data)
{
	struct DLNode* tmp;
	if ((tmp = (struct DLNode*)calloc(1, sizeof(struct DLNode))) == NULL)
		return -1;
	tmp->data = (*l->CTOR)(data);
	if (l->head != NULL)
		tmp->next = l->head, l->head->prev = tmp;
	l->head = tmp;
	if (l->tail == NULL)
		l->tail = tmp;
	l->count++;
	return 0;
}

int WAppendToDList(struct WDLList* l, void* data)
{
	struct DLNode* tmp;
	if ((tmp = (struct DLNode*)calloc(1, sizeof(struct DLNode))) == NULL)
		return -1;
	tmp->data = (*l->CTOR)(data);
	if (l->tail != NULL)
		l->tail->next = tmp, tmp->prev = l->tail;
	l->tail = tmp;
	if (l->head == NULL)
		l->head = tmp;
	l->count++;
	return 0;
}

void WIteratorDList(struct WDLList* l, void (*ITR)(void *))
{
	struct DLNode* p;

	for (p = l->head; p ; p = p->next)
		(*ITR)(p->data);
	return;
}

void WRevIteratorDList(struct WDLList* l, void (*ITR)(void*))
{
	struct DLNode* p;

	for (p = l->tail; p ; p = p->prev)
		(*ITR)(p->data);
	return;
}

void WSortDList(struct WDLList* l)
{
	struct DLNode* p;
	int swap;
	void* tmp;

	do {
		swap = FALSE;
		for (p = l->head; p && p != l->tail; p = p->next)
		{
			if ((*l->CMP)(p->data, p->next->data) > 0)
			{
				swap = TRUE;
				tmp = p->data;
				p->data = p->next->data;
				p->next->data = tmp;
			}
		}
	} while (swap == TRUE);
	return;
}

/* This algo inserts the data to the head of a sorted list,
	then insertion into such a list is O(n) instead of O(n^2)
	to keep the list sorted  */
int WInsertToSortdDList(struct WDLList* l, void* data)
{
	int cond;
	struct DLNode* p;
	void* tmp;

	if ((cond = WPrependToDList(l, data)) != 0)
		return cond;
	for (p = l->head; \
		p && p != l->tail && ((*l->CMP)(p->data, p->next->data) > 0); \
		p = p->next)
	{
		tmp = p->data;
		p->data = p->next->data;
		p->next->data = tmp;
	}
	return 0;
}