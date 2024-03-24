#include <stdlib.h>
#include <c-algorithm.h>

struct WLQueue* WCreateLQueue(int (*CMP)(const void* x, const void* y), \
															void* (*CTOR)(void* x), \
															void (*DTOR)(void* x))
{
	struct WLQueue* que;

	if ((que = (struct WLQueue*)calloc(1, sizeof * que)) == NULL)
		return NULL;

	if ((que->ll = WCreateList(CMP, CTOR, DTOR)) == NULL)
		return NULL;

	return que;
}

void WDeleteLQueue(struct WLQueue* que)
{
	WDeleteList(que->ll);
	free(que);
	return;
}

int WIsEmptyLQueue(struct WLQueue* que)
{
	if (que->ll->count == 0)
		return TRUE;
	return FALSE;
}

unsigned WSizeOfLQueue(struct WLQueue* que)
{
	return que->ll->count;
}

void* WHeadLQueue(struct WLQueue* que)
{
	return (que->ll->head) ? que->ll->head->data : NULL;
}

void* WTailLQueue(struct WLQueue* que)
{
	return (que->ll->tail) ? que->ll->tail->data : NULL;
}

int WEnqueueLQueue(struct WLQueue* que, void* key)
{
	return WAppendToList(que->ll, key);
}

void* WDequeueLQueue(struct WLQueue* que)
{
	void* tmp;

	if (WIsEmptyLQueue(que) == TRUE)
		return NULL;

	tmp = que->ll->CTOR(WHeadLQueue(que));
	WDeleteFromList(que->ll, WHeadLQueue(que)); /* Delete head node with 1st matching key */
	return tmp;
}