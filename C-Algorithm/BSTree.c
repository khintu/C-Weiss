#include <stdlib.h>
#include <c-algorithm.h>

struct WBSTree* WCreateBST(int (*CMP)(const void* x, const void* y),
													void* (*CTOR)(void* x),
													void (*DTOR)(void* x))
{
	struct WBSTree* bst;

	if ((bst = (struct WBSTree*) calloc(1, sizeof(struct WBSTree))) == NULL)
		return NULL;
	bst->CMP = CMP;
	bst->CTOR = CTOR;
	bst->DTOR = DTOR;
	return bst;
}

static void PostOrderWalkAndDelete(struct WBSTree* bst, struct BSTNode* b)
{
	if (b != NULL)
	{
		PostOrderWalkAndDelete(bst, b->left);
		PostOrderWalkAndDelete(bst, b->right);
		bst->DTOR(b->data);
		free(b);
		bst->count--;
	}
	return;
}

void WDeleteBST(struct WBSTree* bst)
{
	PostOrderWalkAndDelete(bst, bst->tree);
	free(bst);
	return;
}

static struct BSTNode* SearchKeyNode(struct WBSTree* bst, void *key)
{
	struct BSTNode* x;
	x = bst->tree;
	while (x != NULL && (*bst->CMP)(key, x->data) != 0)
	{
		if ((*bst->CMP)(key, x->data) < 0)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

void* WSearchKeyBST(struct WBSTree* bst, void* key)
{
	struct BSTNode* x;
	x = SearchKeyNode(bst, key);
	return (x)?x->data:NULL;
}

static struct BSTNode* MinimumOfNode(struct BSTNode* x)
{
	while (x && x->left != NULL)
	{
		x = x->left;
	}
	return x;
}

void* WMinimumBST(struct WBSTree* bst)
{
	struct BSTNode* x;
	return ((x = MinimumOfNode(bst->tree)) != NULL) ? x->data : x;
}

static struct BSTNode* MaximumOfNode(struct BSTNode* x)
{
	while (x && x->right != NULL)
	{
		x = x->right;
	}
	return x;
}

void* WMaximumBST(struct WBSTree* bst)
{
	struct BSTNode* x;
	return ((x = MaximumOfNode(bst->tree)) != NULL) ? x->data : x;
}

/* Lowest ancestor whos right subtree is also an ancestor */
static struct BSTNode* SuccessorOfNode(struct BSTNode* x)
{
	struct BSTNode* y;
	if (x->right != NULL)
		return MinimumOfNode(x->right);
	y = x->parent;
	while (y != NULL && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

/* Lowest ancestor whos left subtree is also an ancestor */
static struct BSTNode* PredecessorOfNode(struct BSTNode* x)
{
	struct BSTNode* y;
	if (x->left != NULL)
		return MaximumOfNode(x->left);
	y = x->parent;
	while (y != NULL && x == y->left)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

static struct BSTNode* LowestCommonAncestor(struct BSTNode* x, struct BSTNode* y)
{
	struct BSTNode* y1;
	while (x != NULL)
	{
		y1 = y->parent;
		while (y1 != NULL)
		{
			if (x == y1)
				return x;
			y1 = y1->parent;
		}
		x = x->parent;
	}
	return NULL;
}

int WInsertKeyBST(struct WBSTree* bst, void* key)
{
	struct BSTNode* n, *y, *x;

	if ((n = (struct BSTNode*)calloc(1, sizeof(struct BSTNode))) == NULL)
		return -1;
	n->data = bst->CTOR(key);
	y = NULL, x = bst->tree;
	while (x != NULL)
	{
		y = x;
		if ((*bst->CMP)(key, x->data) < 0)
			x = x->left;
		else
			x = x->right;
	}
	n->parent = y;
	if (y == NULL) /* Empty tree, 1st node */
		bst->tree = n;
	else if ((*bst->CMP)(key, y->data) < 0)
		y->left = n;
	else
		y->right = n;
	bst->count++;
	return 0;
}

static void TransplantSubtrees(struct WBSTree* bst, struct BSTNode* u, struct BSTNode* v)
{
	if (u->parent == NULL)
		bst->tree = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	if (v != NULL)
		v->parent = u->parent;
	return;
}

int WDeleteKeyBST(struct WBSTree* bst, void* key)
{
	struct BSTNode* z, *y;
	if ((z = SearchKeyNode(bst, key)) == NULL)
		return -2; /* key not found */
	
	if (z->left == NULL)
		TransplantSubtrees(bst, z, z->right);
	else if (z->right == NULL)
		TransplantSubtrees(bst, z, z->left);
	else
	{
		y = MinimumOfNode(z->right);
		if (y->parent != z)
		{
			TransplantSubtrees(bst, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		TransplantSubtrees(bst, z, y);
		y->left = z->left;
		y->left->parent = y;
	}
	bst->DTOR(z->data);
	bst->count--;
	free(z);
	return 0;
}

static void InorderTreeWalk(struct BSTNode* b, void (*ITR)(void*))
{
	if (b != NULL)
	{
		InorderTreeWalk(b->left, ITR);
		(*ITR)(b->data);
		InorderTreeWalk(b->right, ITR);
	}
	return;
}

void WIteratorBST(struct WBSTree* bst, void (*ITR)(void*))
{
	InorderTreeWalk(bst->tree, ITR);
	return;
}