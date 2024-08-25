#include <router_defs.h>
#include <float.h>

static struct PMSTEdge* edgeCtr(struct PMSTEdge* x)
{
	struct PMSTEdge* u;
	u = (struct PMSTEdge*)calloc(1, sizeof * u);
	u->weigth = x->weigth;
	u->v = x->v;
	return u;
}

static void edgeDtr(struct PMSTEdge* x)
{
	free(x);
	return;
}

static int32_t edgeCmp(struct PMSTEdge* x, struct PMSTEdge* y)
{
	if (x->weigth > y->weigth)
		return 1;
	else if (x->weigth < y->weigth)
		return -1;
	else
		return 0;
}

static struct PMSTVertex* vertexCtr(struct PMSTVertex* x)
{
	struct PMSTVertex* v;
	v = (struct PMSTVertex*)calloc(1, sizeof *v);
	v->vrtxId = x->vrtxId;
	v->key = x->key;
	v->p = x->p;
	v->Adj = NULL/* dont create here, in assignPMSTEdgesItr */;
	return v;
}

static void vertexDtr(struct PMSTVertex* x)
{
	if (x->Adj)
		WDeleteList(x->Adj);
	free(x);
	return;
}

static int32_t vertexCmp(struct PMSTVertex* x, struct PMSTVertex* y)
{
	if (x->vrtxId > y->vrtxId)
		return 1;
	else if (x->vrtxId < y->vrtxId)
		return -1;
	else
		return 0;
}

static void createPMSTVertexItr(struct Router* Rtr, struct WLList* vertices)
{
	struct PMSTVertex v = { 0, FLT_MAX, NULL, NULL };
	
	v.vrtxId = Rtr->Id;
	WAppendToList(vertices, &v);
	return;
}

static void assignPMSTEdgesItr(struct Router* Rtr, struct WLList* vertices)
{
	int32_t i;
	struct PMSTEdge e = { 0.0f };
	struct PMSTVertex* x, *u, xKey = { 0 };

	xKey.vrtxId = Rtr->Id;
	u = WFindInList(vertices, &xKey);

	for (i = 0; Rtr->FwdgTbl[i] && i < MAX_FWDGTBL_ENTRIES; ++i) {
		xKey.vrtxId = Rtr->FwdgTbl[i]->I;
		if (x = WFindInList(vertices, &xKey))
			e.v = x;
		e.weigth = Rtr->FwdgTbl[i]->Metric;
		if (u->Adj == NULL)
			u->Adj = WCreateList((WCMPFP)edgeCmp, (WCTRFP)edgeCtr, (WDTRFP)edgeDtr);
		WAppendToList(u->Adj, &e);
	}
	return;
}

struct WLList* initializePMSTGraphContainer(struct WLList* inetList)
{
	struct WLList* Graph;

	Graph = WCreateList((WCMPFP)vertexCmp, (WCTRFP)vertexCtr, (WDTRFP)vertexDtr);

	WIteratorList3(inetList, (void*)Graph, (void(*)(void*, void*))createPMSTVertexItr);
	WIteratorList3(inetList, (void*)Graph, (void(*)(void*, void*))assignPMSTEdgesItr);

	return Graph;
}

void DeletePMSTGraph(struct WLList* Graph)
{
	WDeleteList(Graph);
	return;
}

static struct PMSTVertex* vertexMinPQCtr(struct PMSTVertex* x)
{
	return x;
}

static void vertexMinPQDtr(struct PMSTVertex* x)
{
	return;
}

static int32_t vertexMinPQCmp(struct PMSTVertex* x, struct PMSTVertex* y)
{
	if (x->key > y->key)
		return -1;
	else if (x->key < y->key)
		return 1;
	else
		return 0;
}

static void populateMinPQItr(struct PMSTVertex* v, struct WPAQueue* Q)
{
	WMaxHeapInsertPAQueue(Q, v);
	return;
}

static void decreaseMinPQKey(struct WPAQueue* Q, struct PMSTVertex* key, float weigth)
{
	int32_t i;
	
	if ((i = WHeapFindKeyIndex(Q, key)) >= 0) {
		((struct PMSTVertex*)Q->array[i])->key = weigth;
		WHeapIncKeyPAQueue(Q, i, key);
	}
	return;
}

static void calcLightEdgesItr(struct PMSTEdge* e, struct WPAQueue* Q, struct PMSTVertex *u)
{
	if (WHeapFindKeyIndex(Q, (void*)e->v) >= 0) {
		if (e->weigth < e->v->key) {
			e->v->p = u;
			decreaseMinPQKey(Q, e->v, e->weigth);
		}
	}
	return;
}

static void PrimsMSTMain(struct WLList* G)
{
	struct PMSTVertex* u;
	struct WPAQueue* Q;

	Q = WCreatePAQueue(G->count, (WCMPFP)vertexMinPQCmp, (WCTRFP)vertexMinPQCtr, (WDTRFP)vertexMinPQDtr);
	WIteratorList3(G, (void*)Q, populateMinPQItr);
	while (u = WHeapExtractMaxPAQueue(Q)) {
		if (u->Adj)
			WIteratorList4(u->Adj, (void*)Q, (void*)u, calcLightEdgesItr);
	}
	WDeletePAQueue(Q);
	return;
}

static void printTreeItr(struct PMSTVertex* v)
{
	if (v->p) {
		printf("Edge (%d-%d), weight %g\n", v->p->vrtxId, v->vrtxId, v->key);
	}
	return;
}

static void printMST(struct WLList* G)
{
	WIteratorList(G, printTreeItr);
	return;
}

void FindLightEdgesOnGraphCuts(uint32_t idx, struct WLList* Graph)
{
	struct PMSTVertex* src, srcKey = { 0 };
	srcKey.vrtxId = idx;
	if (!(src = WFindInList(Graph, &srcKey)))
		printf("Source vertex not found in Graph, %u\n", idx);
	else {
		src->key = 0;
		PrimsMSTMain(Graph);
		printMST(Graph);
	}
	return;
}