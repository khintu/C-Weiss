#include <router_defs.h>

struct MSTEdge* MSTEdgeCtr(struct MSTEdge* x)
{
	struct MSTEdge* y;
	if ((y = (struct MSTEdge*)calloc(1, sizeof * y)) != NULL) {
		y->u = x->u;
		y->v = x->v;
		y->weigth = x->weigth;
	}
	return y;
}

void MSTEdgeDtr(struct MSTEdge* x)
{
	free(x);
	return;
}

int32_t MSTEdgeCmp(struct MSTEdge* x, struct MSTEdge* y)
{
	if (x->weigth > y->weigth)
		return 1;
	else if (x->weigth < y->weigth)
		return -1;
	else
		return 0;
}

struct MSTVertex* MSTVertexCtr(struct MSTVertex* x)
{
	struct MSTVertex* y;
	if ((y = (struct MSTVertex*)calloc(1, sizeof * y)) != NULL) {
		y->setNode = x->setNode;
		y->vrtxId = x->vrtxId;
	}
	return y;
}

void MSTVertexDtr(struct MSTVertex* x)
{
	free(x);
	return;
}

int32_t MSTVertexCmp(struct MSTVertex* x, struct MSTVertex* y)
{
	if (x->vrtxId > y->vrtxId)
		return 1;
	else if (x->vrtxId < y->vrtxId)
		return -1;
	else
		return 0;
}

void createMSTVertexItr(struct Router* Rtr, struct WLList* vertices)
{
	struct MSTVertex v = { 0 };
	v.vrtxId = Rtr->Id;
	WAppendToList(vertices, &v);
	return;
}

void createMSTEdgeItr(struct Router* Rtr, struct WLList* vertices, struct WLList* edges)
{
	int32_t i;
	struct MSTEdge e = { 0 };
	struct MSTVertex* x, xKey = { 0 };

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

struct MSTGraph* initializeMSTGraphContainer(struct WLList* inetList)
{
	struct MSTGraph* G;

	if ((G = (struct MSTGraph*)calloc(1, sizeof * G)) == NULL)
		return NULL;

	G->vertices = WCreateList((WCMPFP)MSTVertexCmp, (WCTRFP)MSTVertexCtr, (WDTRFP)MSTVertexDtr);
	G->edges = WCreateList((WCMPFP)MSTEdgeCmp, (WCTRFP)MSTEdgeCtr, (WDTRFP)MSTEdgeDtr);
	
	WIteratorList3(inetList, (void*)G->vertices, (void(*)(void*, void*))createMSTVertexItr);
	WIteratorList4(inetList, (void*)G->vertices, (void*)G->edges,\
								(void(*)(void*, void*, void*))createMSTEdgeItr);
	return G;
}

void DeleteMSTGraph(struct MSTGraph* G)
{
	WDeleteList(G->vertices);
	WDeleteList(G->edges);
	free(G);
	return;
}