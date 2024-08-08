#include <router_defs.h>

static struct MST2Edge* MSTEdgeCtr(struct MST2Edge* x)
{
	struct MST2Edge* y;
	if ((y = (struct MST2Edge*)calloc(1, sizeof * y)) != NULL) {
		y->u = x->u;
		y->v = x->v;
		y->weigth = x->weigth;
	}
	return y;
}

static void MSTEdgeDtr(struct MST2Edge* x)
{
	free(x);
	return;
}

static int32_t MSTEdgeCmp(struct MST2Edge* x, struct MST2Edge* y)
{
	if (x->weigth > y->weigth)
		return 1;
	else if (x->weigth < y->weigth)
		return -1;
	else
		return 0;
}

static struct MST2Vertex* MSTVertexCtr(struct MST2Vertex* x)
{
	struct MST2Vertex* y;
	if ((y = (struct MST2Vertex*)calloc(1, sizeof * y)) != NULL) {
		y->setNode = x->setNode;
		y->vrtxId = x->vrtxId;
	}
	return y;
}

static void MSTVertexDtr(struct MST2Vertex* x)
{
	free(x);
	return;
}

static int32_t MSTVertexCmp(struct MST2Vertex* x, struct MST2Vertex* y)
{
	if (x->vrtxId > y->vrtxId)
		return 1;
	else if (x->vrtxId < y->vrtxId)
		return -1;
	else
		return 0;
}

static void createMSTVertexItr(struct Router* Rtr, struct WLList* vertices)
{
	struct MST2Vertex v = { 0 };
	v.vrtxId = Rtr->Id;
	WAppendToList(vertices, &v);
	return;
}

static void createMSTEdgeItr(struct Router* Rtr, struct WLList* vertices, struct WLList* edges)
{
	int32_t i;
	struct MST2Edge e = { 0 };
	struct MST2Vertex* x, xKey = { 0 };

	for (i = 0; Rtr->FwdgTbl[i] && i < MAX_FWDGTBL_ENTRIES; ++i) {
		xKey.vrtxId = Rtr->Id;
		if (x = WFindInList(vertices, &xKey))
			e.u = x;
		xKey.vrtxId = Rtr->FwdgTbl[i]->I;
		if (x = WFindInList(vertices, &xKey))
			e.v = x;
		e.weigth = Rtr->FwdgTbl[i]->Metric;
		WAppendToList(edges, &e);
	}
	return;
}

struct MST2Graph* initializeMST2GraphContainer(struct WLList* inetList)
{
	struct MST2Graph* G;

	if ((G = (struct MST2Graph*)calloc(1, sizeof * G)) == NULL)
		return NULL;

	G->vertices = WCreateList((WCMPFP)MSTVertexCmp, (WCTRFP)MSTVertexCtr, (WDTRFP)MSTVertexDtr);
	G->edges = WCreateList((WCMPFP)MSTEdgeCmp, (WCTRFP)MSTEdgeCtr, (WDTRFP)MSTEdgeDtr);

	WIteratorList3(inetList, (void*)G->vertices, (void(*)(void*, void*))createMSTVertexItr);
	WIteratorList4(inetList, (void*)G->vertices, (void*)G->edges, \
								(void(*)(void*, void*, void*))createMSTEdgeItr);
	return G;
}

void DeleteMST2Graph(struct MST2Graph* G)
{
	WDeleteList(G->vertices);
	WDeleteList(G->edges);
	free(G);
	return;
}

static void addVertex2DJSet(struct MST2Vertex* v, struct DJSRtCollctn** S)
{
	struct DJSRtNode* x;
	x = (struct DJSRtNode*)calloc(1, sizeof * x);
	DJSRtAdd2Collctn(S, x);
	DJSRtMakeSet(x, v);
	return;
}

static void addEdges2DJSet(struct MST2Edge* x, struct DJSRtCollctn** S)
{
	if (DJSRtFindSet(x->u->setNode) != DJSRtFindSet(x->v->setNode))
		DJSRtUnion(S, DJSRtFindSet(x->u->setNode), DJSRtFindSet(x->v->setNode));
	return;
}


void ConnectedComponentsGraph2(struct MST2Graph* G, struct DJSRtCollctn** S)
{
	WIteratorList3(G->vertices, (void*)S, (void(*)(void*, void*))addVertex2DJSet);
	WIteratorList3(G->edges, (void*)S, (void(*)(void*, void*))addEdges2DJSet);
	return;
}

int32_t isSameConnectedComponent2(struct MST2Vertex* x, struct MST2Vertex* y)
{
	if (DJSRtFindSet(x->setNode) == DJSRtFindSet(y->setNode))
		return TRUE;
	return FALSE;
}

static void printComponentsItr(struct MST2Vertex* x, struct MST2Vertex* y)
{
	if (isSameConnectedComponent2(x, y) == TRUE)
		printf("%d and %d in same component\n", y->vrtxId, x->vrtxId);
	else
		printf("%d and %d NOT in same component\n", y->vrtxId, x->vrtxId);
	return;
}

static void printComponents(struct MST2Vertex* x, struct WLList* vertices)
{
	WIteratorList3(vertices, (void*)x, (void(*)(void*, void*))printComponentsItr);
	return;
}

void printConnectedComponents2(struct MST2Graph* G)
{
	WIteratorList2(G->vertices, (void(*)(void*, void*))printComponents);
	return;
}