#include <stdlib.h>
#include <math.h>
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
	struct FibNode *currNode, *tmp;
	
	currNode = tree->child;
	while (currNode != NULL) {
		if (currNode->child == NULL) {
			fh->DTOR(currNode->data);
			fh->count--;
			tmp = currNode;
			currNode->p->degree--;
			if (currNode->p->degree == 0)
				currNode = NULL;
			else
				currNode = currNode->right;
			free(tmp);
		}
		else {
			tmp = currNode;
			currNode->p->degree--;
			if (currNode->p->degree == 0)
				currNode = NULL;
			else
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
	while (hn != NULL) {
		tmp = hn;
		if (hn == last) {
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
	if (*list == NULL) {
		*list = node;
		(*list)->left = node;
		(*list)->right = node;
	}
	else {
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

	if (fh->min == NULL) {
		fh->rootList = hn;
		hn->left = hn;
		hn->right = hn;
		fh->min = hn;
	}
	else {
		insertFibNode(&fh->rootList, hn);
		if (fh->CMP(key, fh->min->data) < 0)
			fh->min = hn;
	}
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
	else {
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

static struct FibNode* collapseHMin2RtLst(struct FibNode* rtLst, struct FibNode const * const hMin)
{
	struct FibNode* head, *tail, *child, *rtLstHead, *rtLstTail;
	int degree;

	// nullify parent of all children of hMin
	for (child = hMin->child, degree = hMin->degree; child != NULL && degree > 0; child = child->right, degree--)
	{
		child->p = NULL;
	}

	// if hMin is the only tree node, or head of rootList
	if (rtLst == hMin) {
		if (rtLst->left == hMin && hMin->right == rtLst) {
			if (hMin->child != NULL)
				rtLst = hMin->child;
			else
				rtLst = NULL;
		}
		else {
			if (hMin->child != NULL) {
				tail = hMin->child->left;
				hMin->child->left = hMin->left;
				hMin->left->right = hMin->child;
				tail->right = hMin->right;
				hMin->right->left = tail;
				rtLst = hMin->child;
			}
			else {
				hMin->left->right = hMin->right;
				hMin->right->left = hMin->left;
				rtLst = hMin->right;
			}
		}
	}
	else {
		// hMin is inside the rootList or at the end
		rtLstHead = hMin->left;
		rtLstTail = hMin->right;
		if (hMin->child != NULL) {
			head = hMin->child;
			tail = hMin->child->left;
			rtLstHead->right = head;
			rtLstTail->left = tail;
			head->left = rtLstHead;
			tail->right = rtLstTail;
		}
		else {
			rtLstHead->right = hMin->right;
			rtLstTail->left = hMin->left;
		}
	}
	return rtLst;
}

static int getDegreeArraySize(double deg)
{
	return (int)ceil(log(deg) / log(2));
}

static void fibHeapLink(struct WFibHeap* fh, struct FibNode* y, struct FibNode* x)
{
	struct FibNode* tmp;

	// remove y from root list of H
	y->left->right = y->right;
	y->right->left = y->left;
	if (fh->rootList == y) {
		
		fh->rootList = y->right;
	}
	// make y a child of x incrementing x.degree
	if (x->child != NULL) {
		tmp = x->child->left;
		x->child->left = y;
		y->left = tmp;
		tmp->right = y;
		y->right = x->child;
	}
	else {
		y->right = y->left = y;
	}
	y->p = x;
	x->child = y;
	x->degree++;
	// y.mark = FALSE
	y->mark = FALSE;
	return;
}

// Condition: H must contain atleast 2 nodes in the rootlist
static void consolidateFibHeap(struct WFibHeap* fh)
{
	struct FibNode** A, *x, *w, *y, *last, *tmp;
	int ALen, d, i;

	ALen = getDegreeArraySize(fh->count);
	if ((A = (struct FibNode**)calloc(ALen, sizeof(struct FibNode*))) == NULL)
		return; // Try next time, consolidate cannot proceed

	for (w = fh->rootList, last = fh->rootList->left; w != last ;\
			 w = w->right, last = fh->rootList)	{
		x = w;
		d = x->degree;
		while (A[d] != NULL) {
			if (d + 1 >= ALen)
			{
				ALen++;
				A = (struct FibNode**)realloc(A, ALen * sizeof(struct FibNode*));
				A[ALen - 1] = NULL;
			}
			y = A[d];
			if (x == y) break; // x is already in A[d] skip x
			if (fh->CMP(x->data, y->data) > 0) {
				tmp = x, x = y, y = tmp;
				w = x;
			}
			fibHeapLink(fh, y, x);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
	}
	fh->min = NULL; // Reconstruct rootList from A, below
	// Since A now contains lowest keys from rootList, assign H.min from A
	for (i = 0; i < ALen; i++) {
		if (A[i] != NULL) {
			if (fh->min == NULL) {
				fh->rootList = A[i];
				fh->rootList->left = fh->rootList;
				fh->rootList->right = fh->rootList;
				fh->min = A[i];
			}
			else {
				tmp = fh->rootList->left;
				fh->rootList->left = A[i];
				A[i]->right = fh->rootList;
				A[i]->left = tmp;
				tmp->right = A[i];
				fh->rootList = A[i];
				if (fh->CMP(A[i]->data, fh->min->data) < 0)
					fh->min = A[i];
			}
		}
	}
	free(A);
	return;
}

static struct FibNode* extractHMinFrmFibHeap(struct WFibHeap* fh)
{
	struct FibNode* z;

	if ((z = fh->min) != NULL) {
		fh->rootList = collapseHMin2RtLst(fh->rootList, z);
		//if (z == z->right)
		if (fh->rootList == NULL || fh->rootList == fh->rootList->right)
			fh->min = fh->rootList;
		else {
			fh->min = z->right;
			consolidateFibHeap(fh);
		}
		fh->count--;
	}
	return z;
}

void* WExtractMinFrmFibHeap(struct WFibHeap* fh)
{
	void* min;
	struct FibNode* z;

	if (fh->count < 1)
		return NULL;
	if ((z = extractHMinFrmFibHeap(fh)) != NULL) {
		min = fh->CTOR(z->data);
		fh->DTOR(z->data);
		free(z);
	}
	else
		return NULL;
	return min;
}

static struct FibNode* findKeyNodeFrmFibHeap(struct WFibHeap* fh, struct FibNode* list, void* key)
{
	int isEnd;
	struct FibNode* x, *tmp;

	for (isEnd = FALSE, x = list; !isEnd ; x = x->right) {
		if (x == list->left)
			isEnd = TRUE;
		if (fh->CMP(x->data, key) == 0)
			return x;
		else {
			if (x->child != NULL)
				if ((tmp = findKeyNodeFrmFibHeap(fh, x->child, key)) != NULL)
					return tmp;
		}
	}
	return NULL;
}

static void CutXFrmYLstMov2RtLst(struct WFibHeap* fh, struct FibNode* x, struct FibNode* y)
{
	// remove x from the child list of y, decrementing y.degree
	if (x->left == x && x->right == x) {
		y->child = NULL;
		y->degree = 0;
	}
	else {
		x->left->right = x->right;
		x->right->left = x->left;
		y->child = x->right;
		y->degree--;
	}
	// add x to the root list of H (prepend)
	if (fh->rootList->left == fh->rootList && fh->rootList->right == fh->rootList) {
		x->right = x->left = fh->rootList;
		fh->rootList->left = fh->rootList->right = x;
	}
	else {
		x->right = fh->rootList->left->right;
		x->left = fh->rootList->left;
		fh->rootList->left->right = x;
		fh->rootList->left = x;
	}
	fh->rootList = x;
	// Reset x's properties
	x->p = NULL;
	x->mark = FALSE;
	return;
}

static void CascadingCut(struct WFibHeap* fh, struct FibNode* y)
{
	struct FibNode* z;

	z = y->p;
	if (z != NULL) {
		if (y->mark == FALSE)
			y->mark = TRUE;
		else {
			CutXFrmYLstMov2RtLst(fh, y, z);
			CascadingCut(fh, z);
		}
	}
	return;
}

int WDecreaseKeyFibHeap(struct WFibHeap* fh, void* key, void *newKey)
{
	struct FibNode* x, *y;

	if ((x = findKeyNodeFrmFibHeap(fh, fh->rootList, key)) == NULL)
		return -2;
	if (fh->CMP(newKey, x->data) > 0)
		return -9;
	fh->DTOR(x->data);
	x->data = fh->CTOR(newKey);
	y = x->p;
	if (y != NULL && fh->CMP(x->data, y->data) < 0) {
		CutXFrmYLstMov2RtLst(fh, x, y);
		CascadingCut(fh, y);
	}
	if (fh->CMP(x->data, fh->min->data) < 0)
		fh->min = x;
	return 0;
}