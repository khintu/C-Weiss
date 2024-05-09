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
	while (currNode != NULL && currNode->p->degree > 0)
	{
		if (currNode->child == NULL)
		{
			fh->DTOR(currNode->data);
			fh->count--;
			tmp = currNode;
			currNode->p->degree--;
			currNode = currNode->right;
			free(tmp);
		}
		else
		{
			tmp = currNode;
			currNode->p->degree--;
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

static struct FibNode* concatenateFibList(struct FibNode* rtLst1, struct FibNode* rtLst2)
{
	struct FibNode* rtLst, *tmp;

	if (rtLst1 == NULL && rtLst2 == NULL)
		return NULL;
	else if (rtLst1 == NULL)
		return rtLst2;
	else if (rtLst2 == NULL)
		return rtLst1;
	else
	{
		rtLst = rtLst1;
		tmp = rtLst1->left;
		tmp->right = rtLst2;
		rtLst->left = rtLst2->left;
		rtLst->left->right = rtLst;
		rtLst2->left = tmp;
	}
	return rtLst;
}

struct WFibHeap* WUnionFibHeap(struct WFibHeap* fh1, struct WFibHeap* fh2)
{
	struct WFibHeap* fh;
	if ((fh = WCreateFibHeap(fh1->CMP, fh1->CTOR, fh1->DTOR)) == NULL)
		return NULL;
	
	fh->min = fh1->min;
	fh->rootList = concatenateFibList(fh1->rootList, fh2->rootList);
	if ((fh1->min == NULL) || (fh2->min != NULL && fh->CMP(fh2->min->data, fh1->min->data) < 0))
		fh->min = fh2->min;
	fh->count = fh1->count + fh2->count;

	// Destroy input FibHeaps
	fh1->rootList = fh2->rootList = NULL;
	WDeleteFibHeap(fh1);
	WDeleteFibHeap(fh2);
	return fh;
}