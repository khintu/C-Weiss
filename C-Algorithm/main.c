#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __WINDOWS__
#include <Windows.h>
#elif __LINUX__
#include <unistd.h>
#endif
#include <time.h>
#include <c-algorithm.h>

struct UnitRec
{
	char* stmt;
	int count;
	float pay;
};

char* ctor(char* s)
{
	char* p;
	if ((p = (char*)malloc(strlen(s) + 1)) != NULL)
		strcpy(p, s);
	return p;
}

void dtor(char* s)
{
	free(s);
	return;
}

struct UnitRec* ctorRec(struct UnitRec* ur)
{
	struct UnitRec* r;
	if ((r = (struct UnitRec*)malloc(sizeof * r)) != NULL)
	{
		r->stmt = ctor(ur->stmt);
		r->count = ur->count;
		r->pay = ur->pay;
	}
	return r;
}

void dtorRec(struct UnitRec* ur)
{
	dtor(ur->stmt);
	free(ur);
	return;
}

void iterator(char* key)
{
	printf("I:%s\n", key);
#ifdef __WINDOWS__
	Sleep(125);
#elif __LINUX__
	usleep(1000 * 125);
#endif
	return;
}

void iterator1(char* key)
{
	printf("%s,", key);
	return;
}

int UnitTestWLList(int argc, char* argv[])
{
	struct WLList* lst;

	printf("Unit Test Linked List\n");

	lst = WCreateList((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WAddToList(lst, "ONE");
	WAddToList(lst, "TWO");
	WAddToList(lst, "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToList(lst, "ONE");
	WPrependToList(lst, "TWO");
	WPrependToList(lst, "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAppendToList(lst, "ONE");
	WAppendToList(lst, "TWO");
	WAppendToList(lst, "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToList(lst, "ONE");
	WInsertToList(lst, "ONE", "TWO");
	WInsertToList(lst, "TWO", "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToList(lst, "ONE");
	WAppendToList(lst, "TWO");
	WAddToList(lst, "THREE");
	WInsertToList(lst, "THREE", "FOUR");
	printf("Find:%s\n", (char*)WFindInList(lst, "ONE"));
	WIteratorList(lst, (void (*)(void*))iterator);
	WSortList(lst);
	WIteratorList(lst, (void (*)(void*))iterator);
	WInsertToSortdList(lst, "FIVE");
	WInsertToSortdList(lst, "ABC");
	WInsertToSortdList(lst, "XYZ");
	WIteratorList(lst, (void (*)(void*))iterator);
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
	WDeleteFromList(lst, "FOUR");
	WDeleteFromList(lst, "FIVE");
	WDeleteFromList(lst, "ABC");
	WDeleteFromList(lst, "XYZ");
	assert(lst->tail == NULL && lst->head == NULL);

	WInsertToSortdList(lst, "XYZ3");
	WInsertToSortdList(lst, "XYZ2");
	WInsertToSortdList(lst, "XYZ1");
	WIteratorList(lst, (void (*)(void*))iterator);

	WDeleteList(lst);
	return 0;
}

int UnitTestWDLList(int argc, char* argv[])
{
	struct WDLList* lst;

	printf("Unit Test Doubly Linked List\n");

	lst = WCreateDList((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WAddToDList(lst, "ONE");
	WAddToDList(lst, "TWO");
	WAddToDList(lst, "THREE");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToDList(lst, "ONE");
	WAddToDList(lst, "TWO");
	WAddToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToDList(lst, "ONE");
	WPrependToDList(lst, "TWO");
	WPrependToDList(lst, "THREE");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToDList(lst, "ONE");
	WPrependToDList(lst, "TWO");
	WPrependToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAppendToDList(lst, "ONE");
	WAppendToDList(lst, "TWO");
	WAppendToDList(lst, "THREE");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAppendToDList(lst, "ONE");
	WAppendToDList(lst, "TWO");
	WAppendToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToDList(lst, "ONE");
	WInsertToDList(lst, "ONE", "TWO");
	WInsertToDList(lst, "TWO", "THREE");
	WInsertToDList(lst, "THREE", "FOUR");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");
	printf("Find:%s\n", (char*)WFindInDList(lst, "FOUR"));
	WDeleteFromDList(lst, "FOUR");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToDList(lst, "ONE");
	WInsertToDList(lst, "ONE", "TWO");
	WInsertToDList(lst, "TWO", "THREE");
	WInsertToDList(lst, "THREE", "FOUR");
	WDeleteFromDList(lst, "FOUR");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	printf("Find:%s\n", (char*)WFindInDList(lst, "ONE"));
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToDList(lst, "ONE");
	WAppendToDList(lst, "TWO");
	WAddToDList(lst, "THREE");
	WInsertToDList(lst, "THREE", "FOUR");
	printf("Find:%s\n", (char*)WFindInDList(lst, "THREE"));
	WIteratorDList(lst, (void (*)(void*))iterator);
	WSortDList(lst);
	WIteratorDList(lst, (void (*)(void*))iterator);
	WDeleteFromDList(lst, "FOUR");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WInsertToSortdDList(lst, "XYZ3");
	WInsertToSortdDList(lst, "XYZ2");
	WInsertToSortdDList(lst, "XYZ1");
	WIteratorDList(lst, (void (*)(void*))iterator);

	WDeleteDList(lst);
	return 0;
}

int UnitTestWCLList(int argc, char* argv[])
{
	struct WCLList* lst;

	printf("Unit Test Circular Linked List\n");

	lst = WCreateCList((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WAddToCList(lst, "ONE");
	WAddToCList(lst, "TWO");
	WAddToCList(lst, "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToCList(lst, "ONE");
	WAddToCList(lst, "TWO");
	WAddToCList(lst, "THREE");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToCList(lst, "ONE");
	WPrependToCList(lst, "TWO");
	WPrependToCList(lst, "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToCList(lst, "ONE");
	WPrependToCList(lst, "TWO");
	WPrependToCList(lst, "THREE");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAppendToCList(lst, "ONE");
	WAppendToCList(lst, "TWO");
	WAppendToCList(lst, "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAppendToCList(lst, "ONE");
	WAppendToCList(lst, "TWO");
	WAppendToCList(lst, "THREE");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToCList(lst, "ONE");
	WInsertToCList(lst, "ONE", "TWO");
	WInsertToCList(lst, "TWO", "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");
	assert(lst->tail == NULL && lst->head == NULL);

	WAddToCList(lst, "ONE");
	WInsertToCList(lst, "ONE", "TWO");
	WInsertToCList(lst, "TWO", "THREE");
	WInsertToCList(lst, "THREE", "FOUR");
	WDeleteFromCList(lst, "FOUR");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WPrependToCList(lst, "ONE");
	WAppendToCList(lst, "TWO");
	WAddToCList(lst, "THREE");
	WInsertToCList(lst, "THREE", "FOUR");
	printf("Find:%s\n", (char*)WFindInCList(lst, "ONE"));
	WIteratorCList(lst, (void (*)(void*))iterator, FALSE);
	WSortCList(lst);
	WIteratorCList(lst, (void (*)(void*))iterator, FALSE);
	WDeleteFromCList(lst, "FOUR");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WInsertToSortdCList(lst, "XYZ3");
	WInsertToSortdCList(lst, "XYZ2");
	WInsertToSortdCList(lst, "XYZ1");
	WIteratorCList(lst, (void (*)(void*))iterator, FALSE);

	WDeleteCList(lst);
	return 0;
}

int UnitTestWLStack(int argc, char* argv[])
{
	printf("Unit Test List Stack\n");

	struct WLStack* stk;

	stk = WCreateLStack((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("Stack Size: %d\n", WSizeOfLStack(stk));
	WPushLStack(stk, "ONE");
	printf("Stack Size: %d\n", WSizeOfLStack(stk));
	WPushLStack(stk, "TWO");
	printf("Stack Size: %d\n", WSizeOfLStack(stk));
	WPushLStack(stk, "THREE");
	printf("Stack Size: %d\n", WSizeOfLStack(stk));
	WPushLStack(stk, "FOUR");
	printf("Stack Size: %d\n", WSizeOfLStack(stk));
	printf("Stack Top: %s\n", (char*)WTopLStack(stk));
	free(WPopLStack(stk));
	printf("Stack Top: %s\n", (char*)WTopLStack(stk));
	free(WPopLStack(stk));
	printf("Stack Top: %s\n", (char*)WTopLStack(stk));
	free(WPopLStack(stk));
	printf("Stack Top: %s\n", (char*)WTopLStack(stk));
	free(WPopLStack(stk));
	assert(WIsEmptyLStack(stk) == TRUE);
	WDeleteLStack(stk);
	return 0;
}

int UnitTestWLQueue(int argc, char* argv[])
{
	printf("Unit Test List Queue\n");

	struct WLQueue* que;

	que = WCreateLQueue((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "ONE");
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "TWO");
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "THREE");
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "FOUR");
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	printf("Queue Head: %s\n", (char*)WHeadLQueue(que));
	free(WDequeueLQueue(que));
	printf("Queue Head: %s\n", (char*)WHeadLQueue(que));
	free(WDequeueLQueue(que));
	printf("Queue Head: %s\n", (char*)WHeadLQueue(que));
	free(WDequeueLQueue(que));
	printf("Queue Head: %s\n", (char*)WHeadLQueue(que));
	free(WDequeueLQueue(que));
	assert(WIsEmptyLQueue(que) == TRUE);

	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "ONE");
	printf("Queue Tail: %s\n", (char*)WTailLQueue(que));
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "TWO");
	printf("Queue Tail: %s\n", (char*)WTailLQueue(que));
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "THREE");
	printf("Queue Tail: %s\n", (char*)WTailLQueue(que));
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	WEnqueueLQueue(que, "FOUR");
	printf("Queue Tail: %s\n", (char*)WTailLQueue(que));
	printf("Queue Size: %d\n", WSizeOfLQueue(que));
	free(WDequeueLQueue(que));
	free(WDequeueLQueue(que));
	free(WDequeueLQueue(que));
	free(WDequeueLQueue(que));
	assert(WIsEmptyLQueue(que) == TRUE);

	WDeleteLQueue(que);
	return 0;
}

int UnitTestWPLQueue(int argc, char* argv[])
{
	printf("Unit Test Priority List Queue\n");

	struct WPLQueue* pQ;

	pQ = WCreatePLQueue((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ1");
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ2");
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ3");
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ4");
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	assert(WIsEmptyPLQueue(pQ) == TRUE);

	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ4");
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	printf("Priority Queue Tail: %s\n", (char*)WTailPLQueue(pQ));
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ3");
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	printf("Priority Queue Tail: %s\n", (char*)WTailPLQueue(pQ));
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ2");
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	printf("Priority Queue Tail: %s\n", (char*)WTailPLQueue(pQ));
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	WEnqueuePLQueue(pQ, "XYZ1");
	printf("Priority Queue Head: %s\n", (char*)WHeadPLQueue(pQ));
	printf("Priority Queue Tail: %s\n", (char*)WTailPLQueue(pQ));
	printf("Priority Queue Size: %d\n", WSizeOfPLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	free(WDequeuePLQueue(pQ));
	assert(WIsEmptyPLQueue(pQ) == TRUE);

	WDeletePLQueue(pQ);
	return 0;
}

int UnitTestWPAQueue(int argc, char* argv[])
{
	printf("Unit Test MaxHeap Priority Array Queue\n");

	struct WPAQueue* pQ;
	char* tmp;
	pQ = WCreatePAQueue(5, (WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WMaxHeapInsertPAQueue(pQ, "XYZ1");
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WMaxHeapInsertPAQueue(pQ, "XYZ2");
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WHeapIncKeyPAQueue(pQ, 1, "XYZ0");
	printf("PAQ IncKey: %s\n", (char*)WMaximumPAQueue(pQ));
	WHeapIncKeyPAQueue(pQ, 1, "XYZ3");
	printf("PAQ IncKey: %s\n", (char*)WMaximumPAQueue(pQ));
	tmp = WHeapExtractMaxPAQueue(pQ);
	printf("PAQ Extract: %s\n", tmp);
	free(tmp);
	tmp = WHeapExtractMaxPAQueue(pQ);
	printf("PAQ Extract: %s\n", tmp);
	free(tmp);
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WMaxHeapInsertPAQueue(pQ, "XYZ0");
	WMaxHeapInsertPAQueue(pQ, "XYZ1");
	WMaxHeapInsertPAQueue(pQ, "XYZ2");
	WMaxHeapInsertPAQueue(pQ, "XYZ3");
	WMaxHeapInsertPAQueue(pQ, "XYZ4");
	WMaxHeapInsertPAQueue(pQ, "XYZ5");
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WDeletePAQueue(pQ);
	return 0;
}

int UnitTestWArrySortInt(int argc, char* argv[])
{
	int A[11] = { 44, 55,33,22,88,99,121,144,0,11,77 };
	//int A[10] = { 1,1,2,3,44,0,0,3,1,2 };
	WMergeSortInt(A, 0, 10);
	WHeapSortInt(A, 11);
	for (int i = 0; i < 11; ++i)
		printf("%d%c", A[i], (i % 11 != 10) ? ',' : '\n');
	return 0;
}

int UnitTestWArrySortStr(int argc, char* argv[])
{
	char* A[10] = {"99", "88", "77", "66", "55", "44", "33", "22", "11", "0"};
	WMergeSort((void**)A, 0, 9, (WCMPFP)strcmp);
	WHeapSort((void**)A, 10, (WCMPFP)strcmp);
	for (int i = 0; i < 10; ++i)
		printf("%s%c", A[i], (i % 10 != 9) ? ',' : '\n');
	return 0;
}

int UnitTestWDLListQuickSortStr(int argc, char* argv[])
{
	//char* A[10] = { "99", "88", "77", "66", "55", "44", "33", "22", "11", "0" };
	char* A[11] = { "0", "12", "11", "22", "22", "55", "11", "33", "22", "11", "0" };
	struct WDLList* dll = WCreateDList((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	for (int i = 0; i < 11; ++i)
		WAppendToDList(dll, A[i]);
	WQuickSortDList(dll);
	WIteratorDList(dll, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteDList(dll);
	return 0;
}

int hashFn(const char* key)
{
	int i, hash;
	for (i = 0, hash = 0; key[i] != NUL; ++i)
		hash += 31 + key[i];
	return hash;
}

int UnitTestWHashMap(int argc, char* argv[])
{
	struct UnitRec ur;
	struct WHashMap* hmap;

	// Basic tests on insert, search, delete
	hmap = WCreateHashMap(5, (int (*)(const void*))hashFn, (WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeyValHashMap(hmap, "Praveen", "Masters of the Universe");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "Praveen"));
	WInsertKeyValHashMap(hmap, "Praveen", "Zero of the Universe");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "Praveen"));
	WDeleteKeyHashMap(hmap, "Praveen");
	WDeleteHashMap(hmap);

	// Complex Records for key/value
	hmap = WCreateHashMap(5, (int (*)(const void*))hashFn, (WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor, (WCTRFP)ctorRec, (WDTRFP)dtorRec);
	ur.stmt = "'Veggie Lovers' is Good";
	ur.count = 1;
	ur.pay = 1600.0f;
	WInsertKeyValHashMap(hmap, "Praveen", &ur);
	ur.stmt = "'Corn Lovers' is Good";
	ur.count = 6;
	ur.pay = 1200.0f;
	WInsertKeyValHashMap(hmap, "Praveen2", &ur);
	printf("HashMap: %s\n", ((struct UnitRec*)WSearchKeyHashMap(hmap, "Praveen"))->stmt);
	printf("HashMap: %s\n", ((struct UnitRec*)WSearchKeyHashMap(hmap, "Praveen2"))->stmt);
	WDeleteHashMap(hmap);

	// Collision handling through chaining
	hmap = WCreateHashMap(4, (int (*)(const void*))hashFn, (WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeyValHashMap(hmap, "Praveen", "Masters of the Universe");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "Praveen"));
	WInsertKeyValHashMap(hmap, "Parveen", "Zero of the Universe");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "Parveen"));
	WInsertKeyValHashMap(hmap, "neevarP", "Hero of the Universe");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "neevarP"));
	WDeleteKeyHashMap(hmap, "Praveen");
	WDeleteKeyHashMap(hmap, "neevarP");
	WDeleteKeyHashMap(hmap, "Parveen");
	printf("HashMap: %s\n", (char*)WSearchKeyHashMap(hmap, "Parveen"));
	WDeleteHashMap(hmap);

	return 0;
}

int UnitTestWBST(int argc, char* argv[])
{
	struct WBSTree* bst;

	bst = WCreateBST((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("BST Max: %s\n", (char*)WMaximumBST(bst));
	printf("BST Min: %s\n", (char*)WMinimumBST(bst));
	WInsertKeyBST(bst, "XYZ1");
	WInsertKeyBST(bst, "XYZ2");
	WInsertKeyBST(bst, "XYZ3");
	WInsertKeyBST(bst, "XYZ4");
	WInsertKeyBST(bst, "XYZ5");
	WInsertKeyBST(bst, "XYZ6");
	WInsertKeyBST(bst, "XYZ7");
	WInsertKeyBST(bst, "XYZ8");
	WInsertKeyBST(bst, "XYZ9");
	WInsertKeyBST(bst, "XYZ4");
	WIteratorBST(bst, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteKeyBST(bst, "XYZ2");
	WDeleteKeyBST(bst, "XYZ4");
	WIteratorBST(bst, (void (*)(void*))iterator1);
	putchar('\n');
	printf("BST Max: %s\n", (char*)WMaximumBST(bst));
	printf("BST Min: %s\n", (char*)WMinimumBST(bst));
	printf("BST Find: %s\n", (char*)WSearchKeyBST(bst, "XYZ2"));
	printf("BST Find: %s\n", (char*)WSearchKeyBST(bst, "XYZ4"));
	WDeleteBST(bst);

	bst = WCreateBST((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("BST Max: %s\n", (char*)WMaximumBST(bst));
	printf("BST Min: %s\n", (char*)WMinimumBST(bst));
	WInsertKeyBST(bst, "XYZ6");
	WInsertKeyBST(bst, "XYZ5");
	WInsertKeyBST(bst, "XYZ7");
	WInsertKeyBST(bst, "XYZ4");
	WInsertKeyBST(bst, "XYZ3");
	WInsertKeyBST(bst, "XYZ8");
	WInsertKeyBST(bst, "XYZ4");
	WInsertKeyBST(bst, "XYZ2");
	WInsertKeyBST(bst, "XYZ9");
	WInsertKeyBST(bst, "XYZ1");
	WIteratorBST(bst, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteKeyBST(bst, "XYZ2");
	WDeleteKeyBST(bst, "XYZ4");
	WIteratorBST(bst, (void (*)(void*))iterator1);
	putchar('\n');
	printf("BST Max: %s\n", (char*)WMaximumBST(bst));
	printf("BST Min: %s\n", (char*)WMinimumBST(bst));
	printf("BST Find: %s\n", (char*)WSearchKeyBST(bst, "XYZ2"));
	printf("BST Find: %s\n", (char*)WSearchKeyBST(bst, "XYZ4"));
	WDeleteBST(bst);
	return 0;
}

int UnitTestWRBT(int argc, char* argv[])
{
	struct WRBTree* rbt;

	rbt = WCreateRBT((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	printf("RBT Find: %s\n", (char*)WSearchKeyRBT(rbt, "XYZ2"));
	printf("RBT Max: %s\n", (char*)WMaximumRBT(rbt));
	printf("RBT Min: %s\n", (char*)WMinimumRBT(rbt));
	WDeleteKeyRBT(rbt, "XYZ9"); // testcase for key not found
	WInsertKeyRBT(rbt, "XYZ1");
	WInsertKeyRBT(rbt, "XYZ2");
	WInsertKeyRBT(rbt, "XYZ3");
	WInsertKeyRBT(rbt, "XYZ4");
	WInsertKeyRBT(rbt, "XYZ5");
	WInsertKeyRBT(rbt, "XYZ6");
	WInsertKeyRBT(rbt, "XYZ7");
	WInsertKeyRBT(rbt, "XYZ8");
	WInsertKeyRBT(rbt, "XYZ9");
	WDeleteKeyRBT(rbt, "XYZ5");
	WDeleteKeyRBT(rbt, "XYZ4");
	WDeleteKeyRBT(rbt, "XYZ1");
	WDeleteKeyRBT(rbt, "XYZ2");
	WDeleteKeyRBT(rbt, "XYZ3");
	WDeleteKeyRBT(rbt, "XYZ6");
	WDeleteKeyRBT(rbt, "XYZ7");
	WDeleteKeyRBT(rbt, "XYZ8");
	WDeleteKeyRBT(rbt, "XYZ9");
	printf("RBT Max: %s\n", (char*)WMaximumRBT(rbt));
	printf("RBT Min: %s\n", (char*)WMinimumRBT(rbt));
	printf("RBT Find: %s\n", (char*)WSearchKeyRBT(rbt, "XYZ2"));
	WIteratorRBT(rbt, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteRBT(rbt);

	// Testcases for tree balancing (Right chaining)
	printf("RBT: Insertion balancing(Right chaining)\n");
	rbt = WCreateRBT((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeyRBT(rbt, "XYZ1");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ2");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ3");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ4");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ5");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ6");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ7");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ8");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ9");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	printf("RBT: Deletion balancing\n");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ1");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ2");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ3");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ4");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ5");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ6");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ7");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ8");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ9");
	WDeleteRBT(rbt);

	// Testcases for tree balancing (Left chaining)
	printf("RBT: Insertion balancing(Left chainging)\n");
	rbt = WCreateRBT((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeyRBT(rbt, "XYZ9");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ8");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ7");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ6");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ5");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ4");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ3");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ2");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WInsertKeyRBT(rbt, "XYZ1");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	printf("RBT: Deletion balancing\n");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ1");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ2");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ3");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ4");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ5");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ6");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ7");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ8");
	printf("RBT Root: %s\n", (char*)(rbt->tree->data));
	WDeleteKeyRBT(rbt, "XYZ9");
	WDeleteRBT(rbt);
	return 0;
}

int UnitTestWSet(int argc, char* argv[])
{
	struct WSet* a, * b, * c;

	printf("Set: Union, Intersection & Minus operations\n");
	a = WCreateSet((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	b = WCreateSet((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeySet(a, "A");
	WInsertKeySet(a, "B");
	WInsertKeySet(a, "C");
	WInsertKeySet(a, "D");
	WInsertKeySet(b, "D");
	WInsertKeySet(b, "E");
	WInsertKeySet(b, "F");
	WUnionOfSet(a, b, &c);
	WIteratorSet(c, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteSet(c);
	WIntersectionOfSet(a, b, &c);
	WIteratorSet(c, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteSet(c);
	WMinusOfSet(a, b, &c);
	WIteratorSet(c, (void (*)(void*))iterator1);
	putchar('\n');
	WDeleteSet(a);
	WDeleteSet(b);
	WDeleteSet(c);
	return 0;
}

int UnitTestWGraph(int argc, char* argv[])
{
	struct WGraph* G;
	printf("Graph: Insert/Delete Vertex\n");
	G = WCreateGraph((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertVertexToGraph(G, "192.168.235.11");
	WInsertVertexToGraph(G, "192.168.235.12");
	WInsertVertexToGraph(G, "192.168.235.13");
	WInsertVertexToGraph(G, "192.168.235.14");
	WInsertVertexToGraph(G, "192.168.235.15");
	WAddEdgeToGraph(G, "192.168.235.11", "192.168.235.12");
	WAddEdgeToGraph(G, "192.168.235.11", "192.168.235.15");
	WAddEdgeToGraph(G, "192.168.235.12", "192.168.235.13");
	WAddEdgeToGraph(G, "192.168.235.12", "192.168.235.14");
	WAddEdgeToGraph(G, "192.168.235.12", "192.168.235.15");
	WAddEdgeToGraph(G, "192.168.235.13", "192.168.235.14");
	WAddEdgeToGraph(G, "192.168.235.14", "192.168.235.15");
	WBreadthFirstSearchGraph(G, "192.168.235.11", (void (*)(void*))iterator1);

	WDeleteEdgeFrmGraph(G, "192.168.235.11", "192.168.235.12");
	WDeleteVertexFrmGraph(G, "192.168.235.12");
	WDeleteGraph(G);
	return 0;
}

#if 0 /* Moved to Router App Project */
int main(int argc, char* argv[])
{
	clock_t t1, t2;
	t1 = clock();
#ifndef UNIT_TEST_ALGO
	printf("Hello Weiss!!\n");
	UnitTestWLList(argc, argv);
	UnitTestWDLList(argc, argv);
	UnitTestWCLList(argc, argv);
	UnitTestWLStack(argc, argv);
	UnitTestWLQueue(argc, argv);
	UnitTestWPLQueue(argc, argv);
	UnitTestWPAQueue(argc, argv);
	UnitTestWArrySortInt(argc, argv);
	UnitTestWArrySortStr(argc, argv);
	UnitTestWDLListQuickSortStr(argc, argv);
	UnitTestWHashMap(argc, argv);
	UnitTestWBST(argc, argv);
	UnitTestWRBT(argc, argv);
	UnitTestWSet(argc, argv);
	UnitTestWGraph(argc, argv);
#else
	/* ---Put your Algo here to test--- */
	UnitTestWGraph(argc, argv);
#endif /* UNIT_TEST_ALGO */
	// ---Runtime Analysis ---
	t2 = clock();
	printf("Runtime = %ld - %ld = %ld\n", t2, t1, t2 - t1);
	return 0;
}
#endif