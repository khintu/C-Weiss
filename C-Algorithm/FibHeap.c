#include <stdlib.h>
#include <c-algorithm.h>

struct WFibHeap* WCreateFibHeap(int (*CMP)(const void* x, const void* y),
																void* (*CTOR)(void* x),
																void (*DTOR)(void* x))
{
	struct WFibHeap* fh;
	if ((fh = (struct WFibHeap*)calloc(1, sizeof *fh)) == NULL)
		return NULL;

	fh->CMP = CMP;
	fh->CTOR = CTOR;
	fh->DTOR = DTOR;
	return fh;
}

static void deleteFibNode(struct WFibHeap* fh, struct FibNode* tree)
{
	struct FibNode *currNode, *tmp, *last;
	
	currNode = tree->child;
	last = (currNode) ? tree->child->left: NULL;
	while (currNode != NULL)
	{
		if (currNode->child == NULL)
		{
			fh->DTOR(currNode->data);
			fh->count--;
			tmp = currNode;
			if (currNode == last)
			{
				free(tmp);
				break;
			}
			currNode = currNode->right;
			free(tmp);
		}
		else
		{
			tmp = currNode;
			if (currNode == last)
			{
				deleteFibNode(fh, tmp);
				break;
			}
			currNode = currNode->right;
			deleteFibNode(fh, tmp);
			
		}
	}
	fh->DTOR(tree->data);
	fh->count--;
	free(tree);
	return;
}

void WDeleteFibHeap(struct WFibHeap* fh)
{
	struct FibNode* hn, *tmp, *last;

	hn = fh->rootList;
	last = (hn) ? fh->rootList->left: NULL;
	while (hn != NULL)
	{
		tmp = hn;
		if (hn == last)
		{
			deleteFibNode(fh, tmp);
			break;
		}
		hn = hn->right;
		deleteFibNode(fh, tmp);
	}
	free(fh);
	return;
}

static void insertFibNode(struct FibNode** list, struct FibNode* node)
{
	struct FibNode* tmp;
	if (*list == NULL)
	{
		*list = node;
		(*list)->left = node;
		(*list)->right = node;
	}
	else
	{
		tmp = (*list)->left;
		node->right = *list;
		(*list)->left = node;
		tmp->right = node;
		node->left = tmp;
	}
	return;
}

int WInsertKeyFibHeap(struct WFibHeap* fh, void* key)
{
	struct FibNode* hn;
	
	if ((hn = (struct FibNode*)calloc(1, sizeof *hn)) == NULL)
		return -1;
	hn->data = fh->CTOR(key);
	fh->count++;

	if (fh->min == NULL)
	{
		fh->rootList = hn;
		hn->left = hn;
		hn->right = hn;
		fh->min = hn;
	}
	else
	{
		insertFibNode(&fh->rootList, hn);
		if (fh->CMP(key, fh->min->data) < 0)
			fh->min = hn;
	}
	fh->count++;
	return 0;
}