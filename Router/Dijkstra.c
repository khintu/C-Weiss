#include <router_defs.h>
#include <float.h>


struct GVertex* GrphCtrEmpty(struct GVertex* v)
{
	return v;
}

void GrphDtrEmpty(struct GVertex* v)
{
	return;
}

struct GVertex* GrphCtr(struct GVertex* v)
{
	struct GVertex* rv;
	rv = (struct GVertex*)malloc(sizeof * rv);
	if (rv) {
		rv->unvisited = v->unvisited;
		rv->distance = v->distance;
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
	WAppendToList(grph, &v);
	Rtr->super = WFindInList(grph, &v);
	return;
}

struct WLList* initializeGraphContainer(struct WLList* inetList)
{
	struct WLList* grph;
	grph = WCreateList((WCMPFP)GrphCmp, (WCTRFP)GrphCtr, (WDTRFP)GrphDtr);
	WIteratorList3(inetList, (void*)grph, (void (*)(void*, void*))GrphInitItr);

	return grph;
}

struct GVertex* getVertexFrmNextEdge(struct GVertex* src, int32_t *RtrIdx, struct WLQueue *negbhrQ)
{
	struct GVertex *dst = NULL;
	if (src->router->FwdgTbl[*RtrIdx] == NULL || *RtrIdx >= MAX_FWDGTBL_ENTRIES) {
		*RtrIdx = 0;
	}
	else {
		dst = gIntfTbl[src->router->FwdgTbl[*RtrIdx]->I]->super;
		if (dst->unvisited == TRUE) {
			dst->unvisited = FALSE;
			dst->distance = src->distance + src->router->FwdgTbl[*RtrIdx]->Metric;
			WEnqueueLQueue(negbhrQ, (void*)dst);
		}
		else if ((src->distance + src->router->FwdgTbl[*RtrIdx]->Metric) < dst->distance) {
			dst->distance = src->distance + src->router->FwdgTbl[*RtrIdx]->Metric;
			WEnqueueLQueue(negbhrQ, (void*)dst);
		}
		else
			printf("\t\tDistance update for %d not needed,bad distance %g\n", dst->router->Id, \
						(src->distance + src->router->FwdgTbl[*RtrIdx]->Metric));
	}
	return dst;
}

void EachVertexDijikstra(struct GVertex* src)
{
	struct WLQueue* negbhrQ;
	struct GVertex* dst;
	int32_t RtrIdx;

	negbhrQ = WCreateLQueue((WCMPFP)GrphCmp, (WCTRFP)GrphCtrEmpty, (WDTRFP)GrphDtrEmpty);
	RtrIdx = 0;
	do {
		printf("src %d, current distance %g\n", src->router->Id, src->distance);
		while (dst = getVertexFrmNextEdge(src, &RtrIdx, negbhrQ)) {
			printf("\tdst %d, distance calctd %g\n", dst->router->Id, dst->distance);
			++RtrIdx;
		}
	} while (src = WDequeueLQueue(negbhrQ));
	WDeleteLQueue(negbhrQ);
	return;
}

void graphDijikstraCalcDistance(struct WLList* grph, struct WLList* inet, uint32_t srcRtrId)
{
	struct Router srcKey = {0}, * srcRtr;
	struct GVertex *src;
	srcKey.Id = srcRtrId;
	if (!(srcRtr = WFindInList(inet, &srcKey))) {
		printf("RouterId %u, not found\n", srcRtrId);
	}
	else {
		src = (struct GVertex*)srcRtr->super;
		src->distance = 0.0f;
		src->unvisited = FALSE;
		EachVertexDijikstra(src);
	}
	return;
}