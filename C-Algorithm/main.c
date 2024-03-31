#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __WINDOWS__
#include <Windows.h>
#endif
#include <c-algorithm.h>

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

void iterator(char* key)
{
	printf("I:%s\n", key);
	Sleep(125);
}

void iterator1(char* key)
{
	printf("%s,", key);
}

int UnitTestWLList(int argc, char* argv[])
{
	struct WLList* lst;

	printf("Unit Test Linked List\n");

	lst = WCreateList(strcmp, ctor, dtor);
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
	WIteratorList(lst, iterator);
	WSortList(lst);
	WIteratorList(lst, iterator);
	WInsertToSortdList(lst, "FIVE");
	WInsertToSortdList(lst, "ABC");
	WInsertToSortdList(lst, "XYZ");
	WIteratorList(lst, iterator);
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
	WIteratorList(lst, iterator);

	WDeleteList(lst);
	return 0;
}

int UnitTestWDLList(int argc, char* argv[])
{
	struct WDLList* lst;

	printf("Unit Test Doubly Linked List\n");

	lst = WCreateDList(strcmp, ctor, dtor);
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
	WIteratorDList(lst, iterator);
	WSortDList(lst);
	WIteratorDList(lst, iterator);
	WDeleteFromDList(lst, "FOUR");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WInsertToSortdDList(lst, "XYZ3");
	WInsertToSortdDList(lst, "XYZ2");
	WInsertToSortdDList(lst, "XYZ1");
	WIteratorDList(lst, iterator);

	WDeleteDList(lst);
	return 0;
}

int UnitTestWCLList(int argc, char* argv[])
{
	struct WCLList* lst;

	printf("Unit Test Circular Linked List\n");

	lst = WCreateCList(strcmp, ctor, dtor);
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
	WIteratorCList(lst, iterator, FALSE);
	WSortCList(lst);
	WIteratorCList(lst, iterator, FALSE);
	WDeleteFromCList(lst, "FOUR");
	WDeleteFromCList(lst, "THREE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "ONE");
	assert(lst->tail == NULL && lst->head == NULL);

	WInsertToSortdCList(lst, "XYZ3");
	WInsertToSortdCList(lst, "XYZ2");
	WInsertToSortdCList(lst, "XYZ1");
	WIteratorCList(lst, iterator, FALSE);

	WDeleteCList(lst);
	return 0;
}

int UnitTestWLStack(int argc, char* argv[])
{
	printf("Unit Test List Stack\n");

	struct WLStack* stk;

	stk = WCreateLStack(strcmp, ctor, dtor);
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

	que = WCreateLQueue(strcmp, ctor, dtor);
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

	pQ = WCreatePLQueue(strcmp, ctor, dtor);
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

UnitTestWPAQueue(int argc, char* argv[])
{
	printf("Unit Test MaxHeap Priority Array Queue\n");

	struct WPAQueue* pQ;
	char* tmp;
	pQ = WCreatePAQueue(5, strcmp, ctor, dtor);
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
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WMaxHeapInsertPAQueue(pQ, "XYZ0");
	WMaxHeapInsertPAQueue(pQ, "XYZ1");
	WMaxHeapInsertPAQueue(pQ, "XYZ3");
	WMaxHeapInsertPAQueue(pQ, "XYZ4");
	WMaxHeapInsertPAQueue(pQ, "XYZ5");
	WMaxHeapInsertPAQueue(pQ, "XYZ6");
	printf("PAQ Max: %s\n", (char*)WMaximumPAQueue(pQ));
	WDeletePAQueue(pQ);
	return 0;
}

int UnitTestWArrySortInt(int argc, char* argv[])
{
	int A[10] = { 44, 55,33,22,88,99,121,144,0,11 };
	//int A[10] = { 1,1,2,3,44,0,0,3,1,2 };
	WMergeSortInt(A, 0, 9);
	WHeapSortInt(A, 10);
	for (int i = 0; i < 10; ++i)
		printf("%d%c", A[i], (i % 10 != 9) ? ',' : '\n');
	return 0;
}

int UnitTestWArrySortStr(int argc, char* argv[])
{
	char* A[10] = {"99", "88", "77", "66", "55", "44", "33", "22", "11", "0"};
	WMergeSort(A, 0, 9, strcmp);
	WHeapSort(A, 10, strcmp);
	for (int i = 0; i < 10; ++i)
		printf("%s%c", A[i], (i % 10 != 9) ? ',' : '\n');
	return 0;
}

int UnitTestWDLListQuickSortStr(int argc, char* argv[])
{
	//char* A[10] = { "99", "88", "77", "66", "55", "44", "33", "22", "11", "0" };
	char* A[11] = { "0", "12", "11", "22", "22", "55", "11", "33", "22", "11", "0" };
	struct WDLList* dll = WCreateDList(strcmp, ctor, dtor);
	for (int i = 0; i < 11; ++i)
		WAppendToDList(dll, A[i]);
	WQuickSortDList(dll);
	WIteratorDList(dll, iterator1);
	WDeleteDList(dll);
	return 0;
}

int main(int argc, char* argv[])
{
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
	return 0;
}