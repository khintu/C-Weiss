#include <stdlib.h>
#include <c-algorithm.h>


struct WLStack* WCreateLStack(int (*CMP)(const void* x, const void* y), \
															void* (*CTOR)(void* x), \
															void (*DTOR)(void* x))
{
	struct WLStack* stk;

	if ((stk = (struct WLStack*)calloc(1, sizeof(struct WLStack))) == NULL)
		return NULL;

	if ((stk->ll = WCreateList(CMP, CTOR, DTOR)) == NULL)
	{
		free(stk);
		return NULL;
	}

	return stk;
}

void WDeleteLStack(struct WLStack* stk)
{
	WDeleteList(stk->ll);
	free(stk);
	return;
}

int WIsEmptyLStack(struct WLStack* stk)
{
	if (stk->ll->count == 0)
		return TRUE;
	return FALSE;
}

unsigned WSizeOfLStack(struct WLStack* stk)
{
	return stk->ll->count;
}

void* WTopLStack(struct WLStack* stk)
{
	return (stk->ll->head)?stk->ll->head->data:NULL;
}

int WPushLStack(struct WLStack *stk, void *key)
{
	return WPrependToList(stk->ll, key);
}

void* WPopLStack(struct WLStack* stk)
{
	void* tmp;

	if (WIsEmptyLStack(stk) == TRUE)
		return NULL;
	
	tmp = stk->ll->CTOR(WTopLStack(stk));
	WDeleteFromList(stk->ll, WTopLStack(stk)); /* Deletes head node with 1st matching key */
	return tmp;
}