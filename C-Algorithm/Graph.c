#include <stdlib.h>
#include <c-algorithm.h>

struct WGraph* WCreateGraph(int (*CMP)(const void* x, const void* y),
														void* (*CTOR)(void* x),
														void (*DTOR)(void* x))
{
	struct WGraph* G;
	if ((G = (struct WGraph*)calloc(1, sizeof *G)) == NULL)
		return NULL;
	G->CMP = CMP;
	G->CTOR = CTOR;
	G->DTOR = DTOR;
	return G;
}

void WDeleteGraph(struct WGraph* G)
{
	struct Vertex* u, *prevu;
	struct AdjacencyList* e, *preve;
	for (u = G->V; u != NULL; )
	{
		for (e = u->Adj; e != NULL; )
		{
			preve = e;
			e = e->next;
			free(preve);
		}
		(*G->DTOR)(u->data);
		G->count--;
		prevu = u;
		u = u->next;
		free(prevu);
	}
	free(G);
	return;
}

int WInsertVertexToGraph(struct WGraph* G, void* key)
{
	struct Vertex* u, *i;
	if ((u = (struct Vertex*) calloc(1, sizeof*u)) == NULL)
		return -1;
	for (i = G->V; i && i->next != NULL; i = i->next)
		;
	if (G->V == NULL)
		G->V = u;
	else
		i->next = u;
	G->count++;
	u->data = (*G->CTOR)(key);
	return 0;
}

int WDeleteVertexFrmGraph(struct WGraph* G, void* key)
{
	struct Vertex* u, * i, *prevu;
	struct AdjacencyList* e, * preve;

	for (prevu = NULL, u = NULL, i = G->V; i != NULL; prevu = i, i = i->next)
	{
		if ((*G->CMP)(i->data, key) == 0)
		{
			u = i;
			break;
		}
	}
	if (u == NULL) /* key not found */
		return -2;
	for (i = G->V; i != NULL; i = i->next) /* Delete from all Edges/AdjLists */
	{
		if (i == u)
			continue;
		for (preve = NULL, e = i->Adj; \
				e && e->v != u; \
				preve = e, e = e->next)
			;
		if (e != NULL)
		{
			if (i->Adj == e) /* update AdjList head for vertex i */
				i->Adj = e->next;
			else 
				preve->next = e->next;
			free(e);
		}
	}
	(*G->DTOR)(u->data);
	G->count--;
	if (G->V == u) /* Update Vertex list head for G */
		G->V = u->next;
	else
		prevu->next = u->next;
	return 0;
}

int WAddEdgeToGraph(struct WGraph* G, void* uKey, void* vKey)
{

	return 0;
}