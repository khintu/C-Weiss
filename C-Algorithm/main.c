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
	Sleep(500);
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

	printf("Find:%s\n", (char*)WFindInList(lst, "ONE"));
	WSortList(lst);
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

	WSortDList(lst);
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

	printf("Find:%s\n", (char*)WFindInCList(lst, "ONE"));
	WSortCList(lst);
	WIteratorCList(lst, iterator, FALSE);
	WDeleteCList(lst);
	return 0;
}

int main(int argc, char* argv[])
{
	printf("Hello Weiss!!\n");
	
	UnitTestWLList(argc, argv);
	UnitTestWDLList(argc, argv);
	UnitTestWCLList(argc, argv);

	return 0;
}