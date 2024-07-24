#include <router_defs.h>
#include <float.h>

struct GVertex* GrphCtr(struct GVertex* v)
{
	struct GVertex* rv;
	rv = (struct GVertex*)malloc(sizeof * rv);
	if (rv) {
		rv->distance = v->distance;
		rv->unvisited = v->unvisited;
		rv->router = v->router;
	}
	return rv;
}

void GrphDtr(struct GVertex* v)
{
	free(v);
	return;
}

extern int RouterCmp(const struct Router* R1, const struct Router* R2);

int32_t GrphCmp(struct GVertex* x, struct GVertex* y)
{
	return RouterCmp(x->router, y->router);
}

void GrphInitItr(struct Router* Rtr, struct WLList* grph)
{
	struct GVertex v = { TRUE, FLT_MAX, NULL };
	v.router = Rtr;
	Rtr->super = &v;
	WAppendToList(grph, &v);
	return;
}

struct WLList* initializeGraphContainer(struct WLList* inetList)
{
	struct WLList* grph;
	grph = WCreateList((WCMPFP)GrphCmp, (WCTRFP)GrphCtr, (WDTRFP)GrphDtr);
	WIteratorList3(inetList, (void*)grph, (void (*)(void*, void*))GrphInitItr);

	return grph;
}