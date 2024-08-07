#include <stdlib.h>
#include <c-algorithm.h>



struct WLList* WCreateList(int (*CMP)(const void* x, const void* y), \
													void* (*CTOR)(void* x), \
													void (*DTOR)(void* x))
{
	struct WLList* p;

	p = (struct WLList*)calloc(1, sizeof(struct WLList));
	if (p)
		p->CMP = CMP, p->CTOR = CTOR, p->DTOR = DTOR;
	return p;
}

int WAddToList(struct WLList* l, void* data)
{
	struct LNode* p, *tmp;

	for (p = l->head; p && p->next != NULL; p = p->next)
		;
	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return -1;
	
	if (p == NULL)
		p = tmp, l->head = p;
	else
		p->next = tmp, p = tmp;

	l->tail = p;
	l->count++;
	p->data = (*l->CTOR)(data);
	return 0;
}

void WDeleteList(struct WLList* l)
{
	struct LNode* p, *tmp;

	for (p = l->head; p; p = tmp)
	{
		(*l->DTOR)(p->data);
		tmp = p->next;
		free(p);
		l->count--;
	}
	free(l);
	return;
}

void* WFindInList(struct WLList* l, void* key)
{
	struct LNode* p;
	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = p->next)
		;
	return p ? p->data : NULL;
}

int WDeleteFromList(struct WLList* l, void* key)
{
	struct LNode* p, *prev;

	for (prev = p = l->head; p && (*l->CMP)(p->data, key) != 0; prev = p, p = p->next)
		;
	if (p == NULL) /* key not found */
		return -2;

	if (prev != p)
		prev->next = p->next;
	if (l->head == p)
		l->head = p->next;
	if (l->tail == p)
		l->tail = (p != prev) ? prev : NULL;
	(*l->DTOR)(p->data);
	free(p);
	l->count--;
	return 0;
}

int WInsertToList(struct WLList* l, void* key, void* data)
{
	struct LNode* p, *tmp;
	for (p = l->head; p && (*l->CMP)(p->data, key) != 0; p = p->next)
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

int WPrependToList(struct WLList* l, void* data)
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
	l->count++;
	return 0;
}

int WAppendToList(struct WLList* l, void* data)
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
	l->count++;
	return 0;
}

void WIteratorList(struct WLList* l, void (*ITR)(void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data);
	return;
}

void WIteratorList2(struct WLList* l, void (*ITR)(void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, l);
	return;
}

void WIteratorList3(struct WLList* l, void* arg, void (*ITR)(void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, arg);
	return;
}

void WIteratorList4(struct WLList* l, void* arg1, void* arg2, void (*ITR)(void*, void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, arg1, arg2);
	return;
}

void WSortList(struct WLList* l)
{
	struct LNode* p;
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
int WInsertToSortdList(struct WLList* l, void* data)
{
	int cond;
	struct LNode* p;
	void* tmp;

	if ((cond = WPrependToList(l, data)) != 0)
		return cond;
	for (p = l->head;\
			 p && p != l->tail && ((*l->CMP)(p->data, p->next->data) > 0);\
			 p = p->next)
	{
			tmp = p->data;
			p->data = p->next->data;
			p->next->data = tmp;
	}
	return 0;
}