#include <router_defs.h>

void DJSRtAdd2Collctn(struct DJSRtCollctn** cln, struct DJSRtNode* x)
{
	struct DJSRtCollctn* p;
	if (*cln == NULL) {
		*cln = (struct DJSRtCollctn*)calloc(1, sizeof * *cln);
		p = *cln;
	}
	else {
		p = *cln;
		while (p->next != NULL)
			p = p->next;
		p->next = (struct DJSRtCollctn*)calloc(1, sizeof *p);
		p = p->next;
	}
	p->root = x;
	return;
}

void DJSRtDestroyCollctn(struct DJSRtCollctn* cln, struct MST2Graph *G)
{
	struct DJSRtCollctn* p = cln;
	void DSJRtNodeDestroy(struct DJSRtNode*, struct WLList*);
	while (cln != NULL) {
		p = cln;
		cln = cln->next;
		DSJRtNodeDestroy(p->root, G->vertices);
		free(p);
	}
	return;
}

static void DestroyNodeItr(struct MST2Vertex* v, struct DJSRtNode* rt)
{
	struct DJSRtNode* x;
	if (v->setNode && v->setNode != rt && (x = DJSRtFindSet(v->setNode)) == rt) {
		free(v->setNode);
		v->setNode = NULL;
	}
	else if (v->setNode == rt)
		v->setNode = NULL;
	return;
}

void DSJRtNodeDestroy(struct DJSRtNode* rt, struct WLList* vertices)
{
	/* Find vertices in the Graph which have parent as 'x', delete their DJSRtNode reference */
	WIteratorList3(vertices, (void*)rt, DestroyNodeItr);
	free(rt);
	return;
}

void DJSRtMakeSet(struct DJSRtNode* x, struct MST2Vertex* v)
{
	x->p = x;
	x->rank = 0;
	x->vertex = v;
	v->setNode = x;
	return;
}

struct DJSRtNode* DJSRtFindSet(struct DJSRtNode* x)
{
	if (x != x->p)
		x->p = DJSRtFindSet(x->p);
	return x->p;
}

/* return set to remove from collection */
struct DJSRtNode* LinkRootNodes(struct DJSRtNode* x, struct DJSRtNode* y)
{
	if (x->rank > y->rank) {
		y->p = x;
		return y;
	}
	else {
		x->p = y; /* Arbitrary node selection */
		if (x->rank == y->rank)
			y->rank += 1;
		return x;
	}
}

void DJSRtUnion(struct DJSRtCollctn** cln, struct DJSRtNode* x, struct DJSRtNode* y)
{
	struct DJSRtCollctn* p, *prev;
	y = LinkRootNodes(DJSRtFindSet(x), DJSRtFindSet(y));
	for (p = *cln, prev = p; p != NULL && p->root != y; prev = p, p = p->next)
		;
	if (p != NULL) {
		if (p == *cln)
			*cln = p->next;
		else
			prev->next = p->next;
		free(p);
	}
	return;
}