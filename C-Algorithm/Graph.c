#include <stdlib.h>
#include <limits.h>
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
	struct Vertex* u, *i, *previ;
	if ((u = (struct Vertex*) calloc(1, sizeof*u)) == NULL)
		return -1;
	for (i = G->V, previ = NULL; i != NULL ; previ = i, i = i->next)
	{
		if ((*G->CMP)(i->data, key) == 0)
		{
			free(u);
			return -4; /* Only unique keys allowed, key already exists */
		}
	}
	if (G->V == NULL)
		G->V = u;
	else
		previ->next = u;
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
		if (i == u) /* Delete entire AdjList of u */
		{
			for (e = u->Adj; e != NULL; )
			{
				preve = e;
				e = e->next;
				free(preve);
			}
		}
		else
		{
			for (preve = NULL, e = i->Adj; e && e->v != u; preve = e, e = e->next)
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
	}
	(*G->DTOR)(u->data);
	G->count--;
	if (G->V == u) /* Update Vertex list head for G */
		G->V = u->next;
	else
		prevu->next = u->next;
	free(u);
	return 0;
}

int WAddEdgeToGraph(struct WGraph* G, void* uKey, void* vKey)
{
	struct Vertex* u, * i, *v;
	struct AdjacencyList* e, * preve;

	/* 1- locate u in Vertex List, using uKey */
	for (u = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, uKey) == 0)
		{
			u = i;
			break;
		}
	}
	if (u == NULL) /* key not found */
		return -2;
	/* 2- locate v in Vertex List, using vKey */
	for (v = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, vKey) == 0)
		{
			v = i;
			break;
		}
	}
	if (v == NULL) /* key not found */
		return -2;
	/* 3- Check for self loop, u != v */
	if (u == v)
		return -5; /* Self loop not allowed in undirected graph */
	/* 4- Check if edge already exists, return otherwise */
	/* 5- Add edge e in AdjList[u] pointing to v */
	for (preve = NULL, e = u->Adj; e && e->v != v; preve = e, e = e->next)
		;
	if (e != NULL)
		return -6; /* Edge already exists in Adj[u] */
	if ((e = (struct AdjacencyList*) calloc(1, sizeof *e)) == NULL)
		return -1;
	e->v = v;
	if (u->Adj == NULL)
		u->Adj = e;
	else
		preve->next = e;
	/* 6- Add edge e in AdjList[v] pointing to u */
	for (preve = NULL, e = v->Adj; e && e->v != u; preve = e, e = e->next)
		;
	if (e != NULL)
		return -6; /* Edge already exists in Adj[v] */
	if ((e = (struct AdjacencyList*)calloc(1, sizeof * e)) == NULL)
		return -1;
	e->v = u;
	if (v->Adj == NULL)
		v->Adj = e;
	else
		preve->next = e;
	return 0;
}

int WDeleteEdgeFrmGraph(struct WGraph* G, void* uKey, void* vKey)
{
	struct Vertex* u, * i, * v;
	struct AdjacencyList* e, * preve;

	/* 1- locate u in Vertex List, using uKey */
	for (u = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, uKey) == 0)
		{
			u = i;
			break;
		}
	}
	if (u == NULL) /* key not found */
		return -2;
	/* 2- locate v in Vertex List, using vKey */
	for (v = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, vKey) == 0)
		{
			v = i;
			break;
		}
	}
	if (v == NULL) /* key not found */
		return -2;
	/* Delete edge from AdjList[u] */
	for (preve = NULL, e = u->Adj; e && e->v != v; preve = e, e = e->next)
		;
	if (e != NULL)
	{
		if (u->Adj == e) /* update AdjList head for vertex i */
			u->Adj = e->next;
		else
			preve->next = e->next;
		free(e);
	}
	else
		return -7; /* Edge not found */
	/* Delete edge from AdjList[v] */
	for (preve = NULL, e = v->Adj; e && e->v != u; preve = e, e = e->next)
		;
	if (e != NULL)
	{
		if (v->Adj == e) /* update AdjList head for vertex i */
			v->Adj = e->next;
		else
			preve->next = e->next;
		free(e);
	}
	else
		return -7; /* Edge not found */
	return 0;
}

static int GCmp(const struct Vertex* x, const struct Vertex* y)
{
	if (x == y)
		return 0;
	return -1;
}

static void* GCtr(struct Vertex* x)
{
	return x;
}

static void GDtr(struct Vertex* x)
{
	return;
}

int WBreadthFirstSearchGraph(struct WGraph* G, void* key, void (*VISITUFP)(void*,int))
{
	struct Vertex* u, *i;
	struct AdjacencyList* e;
	struct WLQueue* Q;

	if ((Q = WCreateLQueue((WCMPFP)GCmp, (WCTRFP)GCtr, (WDTRFP)GDtr)) == NULL)
		return -1;
	
	/* 1- Locate vertex in G by key */
	for (u = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, key) == 0)
		{
			u = i;
			break;
		}
	}
	if (u == NULL) /* key not found */
		return -2;
	/* 2- Initialize V for BFS */
	for (i = G->V; i != NULL; i = i->next)
	{
		if (i == u)
		{
			i->color = WGRPHCLR_GRAY;
			i->d = 0;
			i->p = NULL;
		}
		else
		{
			i->color = WGRPHCLR_WHITE;
			i->d = INT_MAX;
			i->p = NULL;
		}
	}
	WEnqueueLQueue(Q, u);
	/* 3- Start exploring the BFS frontier vertices, starting from u */
	while ((u = WDequeueLQueue(Q)) != NULL)
	{
		VISITUFP(u->data, u->d);
		for (e = u->Adj; e != NULL; e = e->next)
		{
			if (e->v->color == WGRPHCLR_WHITE)
			{
				e->v->color = WGRPHCLR_GRAY;
				e->v->d = u->d + 1;
				e->v->p = u;
				WEnqueueLQueue(Q, e->v);
			}
		}
		u->color = WGRPHCLR_BLACK;
	}
	WDeleteLQueue(Q);
	return 0;
}

static int PrintPathFrmS2V(struct Vertex* s, struct Vertex* v, void (*VISITUFP)(void*,int))
{
	int c = 0;

	if (v == s)
		(*VISITUFP)(s->data, s->d);
	else if (v->p == NULL)
		return -8; /* BFS tree disconnected */
	else
	{
		if ((c = PrintPathFrmS2V(s, v->p, VISITUFP)) == 0)
			(*VISITUFP)(v->data, v->d);
	}
	return c;
}

void WTraceBFSTreeOnGraph(struct WGraph* G, void* sKey, void* vKey, void (*VISITUFP)(void*,int))
{
	struct Vertex* s, * v, * i;

	/* 1- Locate vertex s in G by key */
	for (s = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, sKey) == 0)
		{
			s = i;
			break;
		}
	}
	if (s == NULL) /* key not found */
		return;
	/* 2- Locate vertex v in G by key */
	for (v = NULL, i = G->V; i != NULL; i = i->next)
	{
		if ((*G->CMP)(i->data, vKey) == 0)
		{
			v = i;
			break;
		}
	}
	if (v == NULL) /* key not found */
		return;
	PrintPathFrmS2V(s, v, VISITUFP);
	return;
}

int WDepthFirstSearchGraph(struct WGraph* G, void (*VISITUFP)(void*, int))
{
	struct Vertex* u, *u1;
	struct AdjacencyList* e;
	struct WLStack* Stk;
	int c, bottom;

	if ((Stk = WCreateLStack((WCMPFP)GCmp, (WCTRFP)GCtr, (WDTRFP)GDtr)) == NULL)
		return -1;

	/* 1- Initialize Graph for search */
	for (u = G->V; u != NULL; u = u->next)
	{
		u->color = WGRPHCLR_WHITE;
		u->p = NULL;
		u->d = INT_MAX;
	}
	/* 2- Start search on each vertex in G */
	for (u = G->V; u != NULL; u = u->next)
	{
		if (u->color == WGRPHCLR_WHITE)
		{
			u->d = 0;
			u->color = WGRPHCLR_GRAY;
			if ((c = WPushLStack(Stk, u)) != 0)
				return c;
			/* 3- Start exploring all of u's edges e */
			while ((u1 = WTopLStack(Stk)) != NULL)
			{
				bottom = TRUE;
				for (e = u1->Adj; e != NULL; e = e->next)
				{
					if (e->v->color == WGRPHCLR_WHITE)
					{
						e->v->color = WGRPHCLR_GRAY;
						e->v->p = u1;
						e->v->d = u1->d + 1;
						if ((c = WPushLStack(Stk, e->v)) != 0)
							return c;
						bottom = FALSE;
					}
				}
				if (bottom == TRUE && u1->color == WGRPHCLR_GRAY)
				{
					u1->color = WGRPHCLR_BLACK;
					(*VISITUFP)(u1->data, u1->d);
					WPopLStack(Stk);
				}
			}
		}
	}
	WDeleteLStack(Stk);
	return 0;
}