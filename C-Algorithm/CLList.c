#include <stdlib.h>
#include <c-algorithm.h>



struct WCLList* WCreateCList(int (*CMP)(const void* x, const void* y), \
														void* (*CTOR)(void* x), \
														void (*DTOR)(void* x))
{
	struct WCLList* p;

	p = (struct WCLList*)calloc(1, sizeof(struct WCLList));
	if (p)
		p->CMP = CMP, p->CTOR = CTOR, p->DTOR = DTOR;
	return p;
}

int WAddToCList(struct WCLList* l, void* data)
{
	struct LNode* p, * tmp;
	/* Break on reaching tail of circular list */
	for (p = l->head; p && p->next != l->head ; p = p->next)
		;
	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return -1;

	if (p == NULL)
		p = tmp, l->head = p;
	else
		p->next = tmp, p = tmp;

	l->tail = p;
	l->tail->next = l->head; /* Make circular */
	l->count++;
	p->data = (*l->CTOR)(data);
	return 0;
}

void WDeleteCList(struct WCLList* l)
{
	struct LNode* p, * tmp;

	/* Break on reaching tail of circular list */
	for (p = l->head; p ; p = tmp)
	{
		(*l->DTOR)(p->data);
		tmp = (p != l->tail) ? p->next : NULL;
		free(p);
		l->count--;
	}
	free(l);
	return;
}

void* WFindInCList(struct WCLList* l, void* key)
{
	struct LNode* p;
	/* Break on reaching tail of circular list */
	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = (p != l->tail)?p->next:NULL)
		;
	return p ? p->data : NULL;
}

int WDeleteFromCList(struct WCLList* l, void* key)
{
	struct LNode* p, * prev;

	/* Break on reaching tail of circular list */
	for (prev = p = l->head;\
			 p && (*l->CMP)(p->data, key) != 0;\
			 prev = p, p = (p != l->tail) ? p->next : NULL)
		;
	if (p == NULL) /* key not found */
		return -2;

	if (prev != p)
		prev->next = p->next;
	if (l->head == p)
		l->head = (p != l->tail) ? p->next : NULL;
	if (l->tail == p)
		l->tail = (p != prev) ? prev : NULL;
	(*l->DTOR)(p->data);
	free(p);
	l->count--;
	return 0;
}

int WInsertToCList(struct WCLList* l, void* key, void* data)
{
	struct LNode* p, * tmp;

	/* Break on reaching tail of circular list */
	for (p = l->head;\
			 p && (*l->CMP)(p->data, key) != 0;\
			 p = (p != l->tail) ? p->next : NULL)
		;
	if (p == NULL) /* key not found */
		return -2;

	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return -1;

	tmp->data = (*l->CTOR)(data);
	tmp->next = p->next;
	p->next = tmp;
	if (p == l->tail)
		l->tail = tmp;
	l->count++;
	return 0;
}

int WPrependToCList(struct WCLList* l, void* data)
{
	struct LNode* tmp;
	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return -1;
	tmp->data = (*l->CTOR)(data);
	if (l->head != NULL)
		tmp->next = l->head;
	l->head = tmp;
	if (l->tail == NULL)
		l->tail = tmp;
	l->tail->next = l->head; /* Make circular */
	l->count++;
	return 0;
}

int WAppendToCList(struct WCLList* l, void* data)
{
	struct LNode* tmp;
	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return -1;
	tmp->data = (*l->CTOR)(data);
	if (l->head == NULL)
		l->head = tmp;
	if (l->tail != NULL)
		l->tail->next = tmp;
	l->tail = tmp;
	l->tail->next = l->head; /* Make circular */
	l->count++;
	return 0;
}

void WIteratorCList(struct WCLList* l, void (*ITR)(void*), int Cycle)
{
	struct LNode* p;

	for (p = l->head; p;\
			 p = (Cycle == TRUE) ? p->next : (p != l->tail) ? p->next : NULL)
		(*ITR)(p->data);
	return;
}

void WSortCList(struct WCLList* l)
{
	struct LNode* p;
	int swap;
	void* tmp;

	do {
		swap = FALSE;
		for (p = l->head; p && p != l->tail; p = (p != l->tail) ? p->next : NULL)
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