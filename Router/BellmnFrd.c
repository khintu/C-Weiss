#include <router_defs.h>

static struct BFVertex* GrphVtxCtr(struct BFVertex* v)
{
	struct BFVertex* rv;
	rv = (struct BFVertex*)malloc(sizeof * rv);
	if (rv) {
		rv->vrtxId = v->vrtxId;
		rv->distance = v->distance;
		rv->router = v->router;
		rv->pred = v->pred;
	}
	return rv;
}

static void GrphVtxDtr(struct BFVertex* v)
{
	free(v);
	return;
}

extern int32_t RouterCmp(const struct Router* R1, const struct Router* R2);

static int32_t GrphVtxCmp(struct BFVertex* x, struct BFVertex* y)
{
	return RouterCmp(x->router, y->router);
}

static struct BFEdge* BFEdgeCtr(struct BFEdge* e)
{
	struct BFEdge* re;
	re = (struct BFEdge*)malloc(sizeof * re);
	if (re) {
		re->weigth = e->weigth;
		re->u = e->u;
		re->v = e->v;
	}
	return re;
}

static void BFEdgeDtr(struct BFEdge* e)
{
	free(e);
	return;
}

static int32_t BFEdgeCmp(struct BFEdge* x, struct BFEdge* y)
{
	if (x->weigth > y->weigth)
		return 1;
	else if (x->weigth < y->weigth)
		return -1;
	else
		return 0;
}

static void createBFVertexItr(struct Router* Rtr, struct WLList* vertices)
{
	struct BFVertex v = { 0, FLT_MAX};
	v.vrtxId = Rtr->Id;
	v.router = Rtr;
	Rtr->super = (void*)WAppendToList2(vertices, &v);
	return;
}

static void createBFEdgeItr(struct Router* Rtr, struct WLList* vertices, struct WLList* edges)
{
	int32_t i;
	struct BFEdge e = { 0 };
	struct BFVertex* x, xKey = { 0 };
	for (i = 0; Rtr->FwdgTbl[i] && i < MAX_FWDGTBL_ENTRIES; ++i) {
		xKey.vrtxId = Rtr->Id;
		xKey.router = Rtr;
		if (x = WFindInList(vertices, &xKey))
			e.u = x;
		xKey.vrtxId = Rtr->FwdgTbl[i]->I;
		xKey.router = gIntfTbl[Rtr->FwdgTbl[i]->I];
		if (x = WFindInList(vertices, &xKey))
			e.v = x;
		e.weigth = Rtr->FwdgTbl[i]->Metric;
		WAppendToList(edges, &e);
	}
	return;
}

struct BFGraph* initializeBFGraphContainer(struct WLList *inet)
{
	struct BFGraph* G;
	
	if ((G = (struct BFGraph*)calloc(1, sizeof * G)) == NULL)
		return NULL;

	G->vertices = WCreateList((WCMPFP)GrphVtxCmp, (WCTRFP)GrphVtxCtr, (WDTRFP)GrphVtxDtr);
	G->edges = WCreateList((WCMPFP)BFEdgeCmp, (WCTRFP)BFEdgeCtr, (WDTRFP)BFEdgeDtr);

	WIteratorList3(inet, (void*)G->vertices, (void(*)(void*, void*))createBFVertexItr);
	WIteratorList4(inet, (void*)G->vertices, (void*)G->edges, (void(*)(void*, void*, void*))createBFEdgeItr);
	return G;
}

void DeleteBFGraph(struct BFGraph* G)
{
	WDeleteList(G->vertices);
	WDeleteList(G->edges);
	free(G);
	return;
}

#define CHK_OVRFLW(id) {\
	feclearexcept(FE_OVERFLOW);\
	expTrm = e->u->distance + e->weigth;\
	if (fetestexcept(FE_OVERFLOW)) {\
		printf("Overflow occurred!"#id);\
		printf("\n");\
		expTrm = FLT_MAX;\
	}\
}

/* Shortest paths in Directed Graph are cycle-free or simple paths only,
	 V-1 length edges */
void graphBellmnFrdCalcDistance(struct BFGraph* G, uint32_t srcRtrId)
{
	struct BFVertex srcKey = { 0 }, * src;
	struct BFEdge* e;
	uint32_t i, j;
	float expTrm = 0.0f;

	srcKey.vrtxId = srcRtrId;
	srcKey.router = gIntfTbl[srcRtrId];
	src = WFindInList(G->vertices, &srcKey);
	if (!src) {
		printf("Source RouterId %u, not found\n", srcRtrId);
		return;
	}
	src->distance = 0.0f;
	for (i = 0; i < G->vertices->count - 1; ++i) {
		for (j = 0; j < G->edges->count; ++j) {
			e = (struct BFEdge*)WGetNthData(G->edges, j);
			CHK_OVRFLW(1)
			if (e->v->distance > expTrm) {
				e->v->distance = expTrm;
				e->v->pred = e->u;
			}
		}
	}

	for (j = 0; j < G->edges->count; ++j) {
		e = (struct BFEdge*)WGetNthData(G->edges, j);
		CHK_OVRFLW(2)
		if (e->v->distance > expTrm) {
			printf("Negative edge cycle detected\n");
			break;
		}
	}
	return;
}

void graphPrintSrc2AllVtxPaths(struct BFGraph* G, uint32_t srcRtrId)
{
	struct BFVertex srcKey = { 0 }, * src, * v;
	uint32_t i;

	srcKey.vrtxId = srcRtrId;
	srcKey.router = gIntfTbl[srcRtrId];
	src = WFindInList(G->vertices, &srcKey);
	if (!src) {
		printf("Source RouterId %u, not found\n", srcRtrId);
		return;
	}

	for (i = 0; i < G->vertices->count; ++i) {
		v = (struct BFVertex*)WGetNthData(G->vertices, i);
		if (v == src)
			continue;
		printf("Path from vertex(AS): %u to source %u, distance: %g\n", v->vrtxId, srcRtrId, v->distance);
		while (v != NULL) {
			printf("\tVertex(AS): %u\n", v->vrtxId);
			v = v->pred;
		}
	}
	return;
}