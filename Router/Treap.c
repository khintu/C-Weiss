#include <randomized.h>

struct WTreap* WCreateTreap(WCTRFP ctr, WDTRFP dtr, WCMPFP cmp)
{
	struct WTreap* tree = (struct WTreap*)calloc(1, sizeof *tree);
	if (tree == NULL)
		return NULL;
	tree->CMP = cmp;
	tree->CTR = ctr;
	tree->DTR = dtr;
	return tree;
}

static struct TreapNode* rotateLeft2Right(struct TreapNode* left, struct TreapNode* right)
{
	struct TreapNode* tmp = left->right;
	left->right = right;
	right->left = tmp;
	return left;
}

static struct TreapNode* rotateRight2Left(struct TreapNode* right, struct TreapNode* left)
{
	struct TreapNode* tmp = right->left;
	right->left = left;
	left->right = tmp;
	return right;
}

static struct TreapNode* insertNode2Treap(struct WTreap* tree, struct TreapNode* root, struct TreapNode* node)
{
	if (root == NULL) {
		return node;
	}
	else if (tree->CMP(node->data, root->data) < 0) {
		root->left = insertNode2Treap(tree, root->left, node);
		if (root->left->priority > root->priority)
			root = rotateLeft2Right(root->left, root);
	}
	else {
		root->right = insertNode2Treap(tree, root->right, node);
		if (root->right->priority > root->priority)
			root = rotateRight2Left(root->right, root);
	}
	return root;
}

int32_t WInsertInTreap(struct WTreap* tree, void* key)
{
	struct TreapNode* node;

	if ((node = (struct TreapNode*)calloc(1, sizeof * node)) == NULL)
		return WENOMEMORY;
	node->data = tree->CTR(key);
	node->priority = WRandom();
	tree->root = insertNode2Treap(tree, tree->root, node);
	tree->count++;
	return WESUCCESS;
}

static struct TreapNode* findKeyInTreap(struct WTreap* tree, struct TreapNode* root, void* key)
{
	struct TreapNode* nodefound;

	if (!root || tree->CMP(root->data, key) == 0)
		return root;
	else if ((nodefound = findKeyInTreap(tree, root->left, key)) ||\
					(nodefound = findKeyInTreap(tree, root->right, key)))
		return nodefound;
	else
		return NULL;
}

void* WSearchInTreap(struct WTreap* tree, void* key)
{
	return findKeyInTreap(tree, tree->root, key);
}

static struct TreapNode* deleteAndAdjustTreap(struct WTreap* tree, struct TreapNode* root, void* key)
{
	struct TreapNode* tmp;
	if (root == NULL)
		return root;
	else if (tree->CMP(root->data, key) == 0) {
		if (root->left == NULL && root->right == NULL) {
			tree->DTR(root->data);
			free(root);
			tree->count--;
			return NULL;
		}
		else if (root->left == NULL) {
			tmp = root->right;
			tree->DTR(root->data);
			free(root);
			tree->count--;
			return tmp;
		}
		else if (root->right == NULL) {
			tmp = root->left;
			tree->DTR(root->data);
			free(root);
			tree->count--;
			return tmp;
		}
		else {
			if (root->left->priority > root->right->priority) {
				root = rotateLeft2Right(root->left, root);
				root->right = deleteAndAdjustTreap(tree, root->right, key);
			}
			else {
				root = rotateRight2Left(root->right, root);
				root->left = deleteAndAdjustTreap(tree, root->left, key);
			}
		}
	}
	else if (tree->CMP(root->data, key) < 0)
		root->right = deleteAndAdjustTreap(tree, root->right, key);
	else
		root->left = deleteAndAdjustTreap(tree, root->left, key);
	return root;
}

int32_t WDeleteFrmTreap(struct WTreap* tree, void* key)
{
	uint32_t count = tree->count;
	if ((tree->root = deleteAndAdjustTreap(tree, tree->root, key)) && count > tree->count\
			|| count > tree->count) {
		return WESUCCESS;
	}
	return WEKEYNOTFND;
}

static void deleteNodeFrmTreap(struct WTreap* tree, struct TreapNode* node)
{
	if (node != NULL) {
		deleteNodeFrmTreap(tree, node->left);
		deleteNodeFrmTreap(tree, node->right);
		tree->DTR(node->data);
		tree->count--;
		free(node);
	}
	return;
}

void WDeleteTreap(struct WTreap* tree)
{
	deleteNodeFrmTreap(tree, tree->root);
	free(tree);
	return;
}

static void iterateTreapNode(struct TreapNode* node, void (*ITR)(void*, uint32_t))
{
	if (node == NULL)
		return;
	iterateTreapNode(node->left, ITR);
	ITR(node->data, node->priority);
	iterateTreapNode(node->right, ITR);
	return;
}

void WIterateTreap(struct WTreap* tree, void (*ITR)(void*))
{
	iterateTreapNode(tree->root, ITR);
	return;
}