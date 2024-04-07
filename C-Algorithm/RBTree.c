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
	rbt->tree = rbt->nil; /* Choosing to initialize empty tree with T.Nil instead of NULL */
	return rbt;
}

static void PostOrderWalkAndDelete(struct WRBTree* rbt, struct RBTNode* x)
{
	if (x != rbt->nil) /* delete internal nodes only */
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
static void LeftRotateNode(struct WRBTree* rbt, struct RBTNode* x)
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
static void RightRotateNode(struct WRBTree* rbt, struct RBTNode* y)
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

static void InsertKeyFixup(struct WRBTree* rbt, struct RBTNode* z)
{
	struct RBTNode* y; /* z's uncle */

	while (z->parent->color == WRBCLR_RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			if (y->color == WRBCLR_RED)
			{
				z->parent->color = WRBCLR_BLACK;
				y->color = WRBCLR_BLACK;
				z->parent->parent->color = WRBCLR_RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					LeftRotateNode(rbt, z);
				}
				z->parent->color = WRBCLR_BLACK;
				z->parent->parent->color = WRBCLR_RED;
				RightRotateNode(rbt, z->parent->parent);
			}
		}
		else /* left excanged with right in else case, including rotations */
		{
			y = z->parent->parent->left;
			if (y->color == WRBCLR_RED)
			{
				z->parent->color = WRBCLR_BLACK;
				y->color = WRBCLR_BLACK;
				z->parent->parent->color = WRBCLR_RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					RightRotateNode(rbt, z);
				}
				z->parent->color = WRBCLR_BLACK;
				z->parent->parent->color = WRBCLR_RED;
				LeftRotateNode(rbt, z->parent->parent);
			}
		}
	}
	rbt->tree->color = WRBCLR_BLACK;
	return;
}

int WInsertKeyRBT(struct WRBTree* rbt, void* key)
{
	struct RBTNode* y, *x, *z;

	if ((z = (struct RBTNode*)calloc(1, sizeof(struct RBTNode))) == NULL)
		return -1;
	z->data = (*rbt->CTOR)(key);
	y = rbt->nil;
	x = rbt->tree;
	while (x != rbt->nil)
	{
		y = x;
		if ((*rbt->CMP)(key, x->data) < 0)
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;
	if (y == rbt->nil)
		rbt->tree = z; /* Empty tree case */
	else if ((*rbt->CMP)(key, y->data) < 0)
		y->left = z;
	else
		y->right = z;
	z->left = rbt->nil;
	z->right = rbt->nil;
	z->color = WRBCLR_RED;
	InsertKeyFixup(rbt, z);
	rbt->count++;
	return 0;
}

static void InorderTreeWalk(struct WRBTree *rbt, struct RBTNode* b, void (*ITR)(void*))
{
	if (b != rbt->nil)
	{
		InorderTreeWalk(rbt, b->left, ITR);
		(*ITR)(b->data);
		InorderTreeWalk(rbt, b->right, ITR);
	}
	return;
}

void WIteratorRBT(struct WRBTree* rbt, void (*ITR)(void*))
{
	InorderTreeWalk(rbt, rbt->tree, ITR);
	return;
}

static struct RBTNode* SearchKeyNode(struct WRBTree* rbt, void* key)
{
	struct RBTNode* x;
	x = rbt->tree;
	while (x != rbt->nil && (*rbt->CMP)(key, x->data) != 0)
	{
		if ((*rbt->CMP)(key, x->data) < 0)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

void* WSearchKeyRBT(struct WRBTree* rbt, void* key)
{
	struct RBTNode* x;
	x = SearchKeyNode(rbt, key);
	return (x != rbt->nil) ? x->data : NULL;
}

static struct RBTNode* MinimumOfNode(struct WRBTree* rbt, struct RBTNode* x)
{
	while (x != rbt->nil && x->left != rbt->nil)
	{
		x = x->left;
	}
	return x;
}

void* WMinimumRBT(struct WRBTree* rbt)
{
	struct RBTNode* x;
	return ((x = MinimumOfNode(rbt, rbt->tree)) != rbt->nil) ? x->data : NULL;
}

static struct RBTNode* MaximumOfNode(struct WRBTree* rbt, struct RBTNode* x)
{
	while (x != rbt->nil && x->right != rbt->nil)
	{
		x = x->right;
	}
	return x;
}

void* WMaximumRBT(struct WRBTree* rbt)
{
	struct RBTNode* x;
	return ((x = MaximumOfNode(rbt, rbt->tree)) != rbt->nil) ? x->data : NULL;
}

static void TransplantSubtrees(struct WRBTree* rbt, struct RBTNode* u, struct RBTNode* v)
{
	if (u->parent == rbt->nil)
		rbt->tree = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
	return;
}

static void DeleteKeyFixup(struct WRBTree* rbt, struct RBTNode* x)
{
	struct RBTNode* w;
	while (x != rbt->tree && x->color == WRBCLR_BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == WRBCLR_RED)
			{
				w->color = WRBCLR_BLACK;
				x->parent->color = WRBCLR_RED;
				LeftRotateNode(rbt, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == WRBCLR_BLACK && w->right->color == WRBCLR_BLACK)
			{
				w->color = WRBCLR_RED;
				x = x->parent;
			}
			else
			{
				if (w->right->color == WRBCLR_BLACK)
				{
					w->left->color = WRBCLR_BLACK;
					w->color = WRBCLR_RED;
					RightRotateNode(rbt, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = WRBCLR_BLACK;
				w->right->color = WRBCLR_BLACK;
				LeftRotateNode(rbt, x->parent);
				x = rbt->tree;
			}
		}
		else // exchange right with left including rotations
		{
			w = x->parent->left;
			if (w->color == WRBCLR_RED)
			{
				w->color = WRBCLR_BLACK;
				x->parent->color = WRBCLR_RED;
				RightRotateNode(rbt, x->parent);
				w = x->parent->left;
			}
			if (w->right->color == WRBCLR_BLACK && w->left->color == WRBCLR_BLACK)
			{
				w->color = WRBCLR_RED;
				x = x->parent;
			}
			else
			{
				if (w->left->color == WRBCLR_BLACK)
				{
					w->right->color = WRBCLR_BLACK;
					w->color = WRBCLR_RED;
					LeftRotateNode(rbt, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = WRBCLR_BLACK;
				w->left->color = WRBCLR_BLACK;
				RightRotateNode(rbt, x->parent);
				x = rbt->tree;
			}
		}
	}
	x->color = WRBCLR_BLACK;
	return;
}

int WDeleteKeyRBT(struct WRBTree* rbt, void* key)
{
	struct RBTNode* z, *y, *x;
	int y_original_color;

	if ((z = WSearchKeyRBT(rbt, key)) == NULL)
		return -2; /* Key not found */

	y = z;
	y_original_color = y->color;
	if (z->left == rbt->nil)
	{
		x = z->right;
		TransplantSubtrees(rbt, z, z->right);
	}
	else if (z->right == rbt->nil)
	{
		x = z->left;
		TransplantSubtrees(rbt, z, z->left);
	}
	else
	{
		y = MinimumOfNode(rbt, z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else
		{
			TransplantSubtrees(rbt, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		TransplantSubtrees(rbt, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if (y_original_color == WRBCLR_BLACK)
		DeleteKeyFixup(rbt, x);
	rbt->DTOR(z->data);
	rbt->count--;
	free(z);
	return 0;
}