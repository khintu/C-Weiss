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
		rv->prev = v->prev;
	}
	return rv;
}

void GrphDtr(struct GVertex* v)
{
	free(v);
	return;
}

extern int32_t RouterCmp(const struct Router* R1, const struct Router* R2);

int32_t GrphCmp(struct GVertex* x, struct GVertex* y)
{
	return RouterCmp(x->router, y->router);
}

void GrphInitItr(struct Router* Rtr, struct WLList* grph)
{
	struct GVertex v = { TRUE, FLT_MAX, NULL, NULL };
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
			dst->prev = src;
			WEnqueueLQueue(negbhrQ, (void*)dst);
		}
		else if ((src->distance + src->router->FwdgTbl[*RtrIdx]->Metric) < dst->distance) {
			dst->distance = src->distance + src->router->FwdgTbl[*RtrIdx]->Metric;
			dst->prev = src;
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
		printf("Source RouterId %u, not found\n", srcRtrId);
	}
	else {
		src = (struct GVertex*)srcRtr->super;
		src->distance = 0.0f;
		src->unvisited = FALSE;
		EachVertexDijikstra(src);
	}
	return;
}

void graphTraceShortstPathFrmSrc2Trgt(struct WLList* grph, struct WLList* inet, uint32_t trgtRtrId)
{
	struct Router trgtKey = { 0 }, * trgtRtr;
	struct GVertex* trgt;
	trgtKey.Id = trgtRtrId;
	if (!(trgtRtr = WFindInList(inet, &trgtKey))) {
		printf("Target RouterId %u, not found\n", trgtRtrId);
	}
	else {
		printf("Tracing shortest path from target %u to source\n", trgtRtrId);
		trgt = (struct GVertex*)trgtRtr->super;
		do {
			printf("AS:%u @ Distance %g\n", trgt->router->Id, trgt->distance);
		} while (trgt = trgt->prev);
	}
	return;
}

/* --- Dijkstra's Algorithm with MinPriorityQueue optimization --- */

int32_t GrphCmpDist(struct GVertex* x, struct GVertex* y)
{
	if (x->distance > y->distance)
		return 1;
	else if (x->distance < y->distance)
		return -1;
	else
		return 0;
}

void GrphDistUpdt(struct GVertex* target, struct GVertex* src)
{
	target->distance = src->distance;
	return;
}

struct GVertex* getVertexFrmNextEdge2(struct GVertex* src, int32_t* RtrIdx, struct WFibHeap* unvisitedSet)
{
	struct GVertex* dst = NULL;
	struct GVertex newKey = { 0 };
	if (src->router->FwdgTbl[*RtrIdx] == NULL || *RtrIdx >= MAX_FWDGTBL_ENTRIES) {
		*RtrIdx = 0;
	}
	else {
		dst = gIntfTbl[src->router->FwdgTbl[*RtrIdx]->I]->super;
		if (dst->unvisited == TRUE) {
			dst->unvisited = FALSE;
			newKey.distance = src->distance + src->router->FwdgTbl[*RtrIdx]->Metric;
			WDecreaseKeyFibHeap2(unvisitedSet, (void*)dst, (WCMPFP) GrphCmp, \
													(void (*)(void*,void*))GrphDistUpdt, &newKey);
			dst->prev = src;
		}
		else if ((src->distance + src->router->FwdgTbl[*RtrIdx]->Metric) < dst->distance) {
			newKey.distance = src->distance + src->router->FwdgTbl[*RtrIdx]->Metric;
			WDecreaseKeyFibHeap2(unvisitedSet, (void*)dst, (WCMPFP)GrphCmp, \
													(void (*)(void*, void*))GrphDistUpdt, &newKey);
			dst->prev = src;
		}
		else
			printf("\t\tDistance update for %d not needed,bad distance %g\n", dst->router->Id, \
				(src->distance + src->router->FwdgTbl[*RtrIdx]->Metric));
	}
	return dst;
}

void EachVertexDijikstra2(struct GVertex* src, struct WFibHeap* unvisitedSet)
{
	struct GVertex* dst;
	int32_t RtrIdx;

	do {
		RtrIdx = 0;
		printf("src %d, current distance %g\n", src->router->Id, src->distance);
		while (dst = getVertexFrmNextEdge2(src, &RtrIdx, unvisitedSet)) {
			printf("\tdst %d, distance calctd %g\n", dst->router->Id, dst->distance);
			RtrIdx++;
		}
	} while (src = WExtractMinFrmFibHeap(unvisitedSet));

	return;
}

void GrphInitItrDist(struct GVertex* v, struct WFibHeap* unvisitedSet)
{
	WInsertKeyFibHeap(unvisitedSet, (void*)v);
	return;
}

/* The algo converges on shortest path vertices faster than longer paths vertices,
	 Thus, longer paths stay in the MinPQ until all other shorter connected vertices
	 are visited and their distance computed first. Once a vertex is exrtacted it
	 must have been at the shortest distance from source. If not, then it stays below
	 other vertices in the MinPQ until all other paths have been computed and extracted.
	 This makes sure that alternate paths correctly update vertices that may not yet be
	 at optimal shortest distance from source.
*/
void graphDijikstraCalcDistance2(struct WLList* grph, struct WLList* inet, uint32_t srcRtrId)
{
	struct Router srcKey = { 0 }, * srcRtr;
	struct GVertex* src;
	struct WFibHeap* unvisitedSet;
	srcKey.Id = srcRtrId;
	if (!(srcRtr = WFindInList(inet, &srcKey))) {
		printf("RouterId %u, not found\n", srcRtrId);
	}
	else {
		src = (struct GVertex*)srcRtr->super;
		src->distance = 0.0f;
		src->unvisited = FALSE;
		unvisitedSet = WCreateFibHeap((WCMPFP)GrphCmpDist, (WCTRFP)GrphCtrEmpty, (WDTRFP)GrphDtrEmpty);
		WIteratorList3(grph, (void*)unvisitedSet, (void (*)(void*, void*))GrphInitItrDist);
		EachVertexDijikstra2(WExtractMinFrmFibHeap(unvisitedSet), unvisitedSet);
		WDeleteFibHeap(unvisitedSet);
	}
	return;
}