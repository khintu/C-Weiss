#include <stdlib.h>
#include <c-algorithm.h>

struct WPLQueue* WCreatePLQueue(int (*CMP)(const void* x, const void* y), \
															 void* (*CTOR)(void* x), \
															 void (*DTOR)(void* x))
{
	struct WPLQueue* pQ;

	if ((pQ = (struct WPLQueue*)calloc(1, sizeof(struct WPLQueue))) == NULL)
		return NULL;

	if ((pQ->dll = WCreateDList(CMP, CTOR, DTOR)) == NULL)
	{
		free(pQ);
		return NULL;
	}

	return pQ;
}

void WDeletePLQueue(struct WPLQueue* pQ)
{
	WDeleteDList(pQ->dll);
	free(pQ);
	return;
}

int WIsEmptyPLQueue(struct WPLQueue* pQ)
{
	if (pQ->dll->count == 0)
		return TRUE;
	return FALSE;
}

unsigned WSizeOfPLQueue(struct WPLQueue* pQ)
{
	return pQ->dll->count;
}

void* WHeadPLQueue(struct WPLQueue* pQ)
{
	return (pQ->dll->head) ? pQ->dll->head->data : NULL;
}

void* WTailPLQueue(struct WPLQueue* pQ)
{
	return (pQ->dll->tail) ? pQ->dll->tail->data : NULL;
}

int WEnqueuePLQueue(struct WPLQueue* pQ, void* key)
{
	return WInsertToSortdDList(pQ->dll, key);
}

void* WDequeuePLQueue(struct WPLQueue* pQ)
{
	void* tmp;

	if (WIsEmptyPLQueue(pQ) == TRUE)
		return NULL;

	tmp = pQ->dll->CTOR(WHeadPLQueue(pQ));
	WDeleteFromDList(pQ->dll, WHeadPLQueue(pQ)); /* Delete head node with 1st matching key */
	return tmp;
}