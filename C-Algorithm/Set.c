#include <stdlib.h>
#include <c-algorithm.h>

struct WSet* WCreateSet(int (*CMP)(const void* x, const void* y),
												void* (*CTOR)(void* x),
												void (*DTOR)(void* x))
{
	struct WSet* set;

	if ((set = (struct WSet*)calloc(1, sizeof *set)) == NULL)
		return NULL;
	if ((set->rbt = WCreateRBT(CMP, CTOR, DTOR)) == NULL)
	{
		free(set);
		return NULL;
	}
	return set;
}

void WDeleteSet(struct WSet* set)
{
	WDeleteRBT(set->rbt);
	free(set);
	return;
}

int WInsertKeySet(struct WSet* set, void* key)
{
	if (WSearchKeyRBT(set->rbt, key) != NULL)
	{
		return -4; /* Only unique keys allowed, key already exists */
	}
	return WInsertKeyRBT(set->rbt, key);
}

int WDeleteKeySet(struct WSet* set, void* key)
{
	return WDeleteKeyRBT(set->rbt, key);
}

static int IterateOnTree(struct WRBTree* s, struct RBTNode* x, struct WRBTree* u)
{
	int c;
	if (x != s->nil)
	{
		if ((c = IterateOnTree(s, x->left, u)) != 0)
			return c;
		/* Skip keys that are already in u, keeping set keys unique */
		if (WSearchKeyRBT(u, x->data) == NULL)
		{
			if ((c = WInsertKeyRBT(u, x->data)) != 0)
				return c;
		}
		if ((c = IterateOnTree(s, x->right, u)) != 0)
			return c;
	}
	return 0;
}

static int UnionOfSets(struct WRBTree* s, struct WRBTree* t, struct WRBTree* u)
{
	int c;
	if ((c = IterateOnTree(s, s->tree, u)) != 0)
		return c;
	if ((c = IterateOnTree(t, t->tree, u)) != 0)
		return c;
	return 0;
}

int WUnionOfSet(struct WSet* s, struct WSet* t, struct WSet** u)
{
	if ((*u = WCreateSet(s->rbt->CMP, s->rbt->CTOR, s->rbt->DTOR)) == NULL)
		return -1;

	return UnionOfSets(s->rbt, t->rbt, (*u)->rbt);
}

int WIntersectionOfSet(struct WSet* s, struct WSet* t, struct WSet** i);
int WMinusOfSet(struct WSet* s, struct WSet* t, struct WSet** m);

void  WIteratorSet(struct WSet* set, void (*ITR)(void*))
{
	WIteratorRBT(set->rbt, (void (*)(void*))ITR);
	return;
}