#include <stdlib.h>
#include <c-algorithm.h>

static int IterateOnSet(struct WRBTree* s, struct RBTNode* x, struct WGraph* G)
{
	int c;
	if (x != s->nil)
	{
		if ((c = IterateOnSet(s, x->left, G)) != 0)
			return c;
		
		if ((c = WInsertVertexToGraph(G, x->data)) != 0)
			return c;

		if ((c = IterateOnSet(s, x->right, G)) != 0)
			return c;
	}
	return 0;
}


int WConvertSetToGraph(struct WSet* S, struct WGraph** G)
{
	int c;
	if ((*G = WCreateGraph(S->rbt->CMP, S->rbt->CTOR, S->rbt->DTOR)) == NULL)
		return -1;
	if ((c = IterateOnSet(S->rbt, S->rbt->tree, *G)) != 0)
		return c;
	return 0;
}

int WConvertSetToGraph2(struct WSet* S, struct WGraph** G, WCTRFP ctr, WDTRFP dtr)
{
	int c;
	if ((*G = WCreateGraph(S->rbt->CMP, ctr, dtr)) == NULL)
		return -1;
	if ((c = IterateOnSet(S->rbt, S->rbt->tree, *G)) != 0)
		return c;
	return 0;
}