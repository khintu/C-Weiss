#include <stdlib.h>
#include <c-algorithm.h>

struct WRBTree* WCreateRBT(int (*CMP)(const void* x, const void* y),
													 void* (*CTOR)(void* x),
													 void (*DTOR)(void* x))
{
	struct WRBTree* rbt;

	if ((rbt = (struct WRBTree*)calloc(1, sizeof(struct WRBTree))) == NULL)
		return NULL;

	rbt->CMP = CMP;
	rbt->CTOR = CTOR;
	rbt->DTOR = DTOR;
	if ((rbt->nil = (struct RBTNode*)calloc(1, sizeof(struct RBTNode))) == NULL)
	{
		free(rbt);
		return NULL;
	}
	rbt->nil->color = WRBCLR_BLACK; /* T.Nil is not included in the node count of T */
	return rbt;
}

static void PostOrderWalkAndDelete(struct WRBTree* rbt, struct RBTNode* x)
{
	if (x != NULL && (x != rbt->nil)) /* delete internal nodes only */
	{
		PostOrderWalkAndDelete(rbt, x->left);
		PostOrderWalkAndDelete(rbt, x->right);
		rbt->DTOR(x->data);
		free(x);
		rbt->count--;
	}
	return;
}

void WDeleteRBT(struct WRBTree* rbt)
{
	PostOrderWalkAndDelete(rbt, rbt->tree);
	free(rbt->nil);
	free(rbt);
	return;
}

/* X node becomes left node of its right child Y */
static void RotateNodeLeft(struct WRBTree* rbt, struct RBTNode* x)
{
	struct RBTNode* y;

	if ((y = x->right) == rbt->nil)
		return;
	x->right = y->left;
	if (y->left != rbt->nil)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == rbt->nil)
		rbt->tree = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
	return;
}

/* Y node becomes right node of its left child X */
static void RotateNodeRight(struct WRBTree* rbt, struct RBTNode* y)
{
	struct RBTNode* x;

	if ((x = y->left) == rbt->nil)
		return;
	y->left = x->right;
	if (x->right != rbt->nil)
		x->right->parent = y;
	x->parent = y->parent;
	if (y->parent == rbt->nil)
		rbt->tree = x;
	else if (y == y->parent->right)
		y->parent->right = x;
	else
		y->parent->left = x;
	x->right = y;
	y->parent = x;
	return;
}