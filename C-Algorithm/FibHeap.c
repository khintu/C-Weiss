#include <stdlib.h>
#include <c-algorithm.h>

struct WFibHeap* WCreateFibHeap(int (*CMP)(const void* x, const void* y),
																void* (*CTOR)(void* x),
																void (*DTOR)(void* x))
{
	struct WFibHeap* fh;
	if ((fh = (struct WFibHeap*)calloc(1, sizeof *fh)) == NULL)
		return NULL;

	fh->CMP = CMP;
	fh->CTOR = CTOR;
	fh->DTOR = DTOR;
	return fh;
}

void WDeleteFibHeap(struct WFibHeap* fh)
{

	return;
}