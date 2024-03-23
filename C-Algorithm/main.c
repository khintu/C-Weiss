#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

	WPrependToList(lst, "ONE");
	WPrependToList(lst, "TWO");
	WPrependToList(lst, "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");

	WAppendToList(lst, "ONE");
	WAppendToList(lst, "TWO");
	WAppendToList(lst, "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");

	WAddToList(lst, "ONE");
	WInsertToList(lst, "ONE", "TWO");
	WInsertToList(lst, "TWO", "THREE");
	WDeleteFromList(lst, "ONE");
	WDeleteFromList(lst, "TWO");
	WDeleteFromList(lst, "THREE");
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

	WAddToDList(lst, "ONE");
	WAddToDList(lst, "TWO");
	WAddToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");

	WPrependToDList(lst, "ONE");
	WPrependToDList(lst, "TWO");
	WPrependToDList(lst, "THREE");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");

	WPrependToDList(lst, "ONE");
	WPrependToDList(lst, "TWO");
	WPrependToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");

	WAppendToDList(lst, "ONE");
	WAppendToDList(lst, "TWO");
	WAppendToDList(lst, "THREE");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");

	WAppendToDList(lst, "ONE");
	WAppendToDList(lst, "TWO");
	WAppendToDList(lst, "THREE");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "ONE");

	WAddToDList(lst, "ONE");
	WInsertToDList(lst, "ONE", "TWO");
	WInsertToDList(lst, "TWO", "THREE");
	WInsertToDList(lst, "THREE", "FOUR");
	WDeleteFromDList(lst, "ONE");
	WDeleteFromDList(lst, "TWO");
	WDeleteFromDList(lst, "THREE");
	
	printf("Find:%s\n", (char*)WFindInDList(lst, "FOUR"));
	WDeleteFromDList(lst, "FOUR");

	WAddToDList(lst, "ONE");
	WInsertToDList(lst, "ONE", "TWO");
	WInsertToDList(lst, "TWO", "THREE");
	WInsertToDList(lst, "THREE", "FOUR");
	WDeleteFromDList(lst, "FOUR");
	WDeleteFromDList(lst, "THREE");
	WDeleteFromDList(lst, "TWO");

	printf("Find:%s\n", (char*)WFindInDList(lst, "ONE"));
	WDeleteFromDList(lst, "ONE");
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
	
	WPrependToCList(lst, "ONE");
	WPrependToCList(lst, "TWO");
	WPrependToCList(lst, "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");

	WAppendToCList(lst, "ONE");
	WAppendToCList(lst, "TWO");
	WAppendToCList(lst, "THREE");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");

	WAddToCList(lst, "ONE");
	WInsertToCList(lst, "ONE", "TWO");
	WInsertToCList(lst, "TWO", "THREE");
	WDeleteFromCList(lst, "UPFC");
	WDeleteFromCList(lst, "ONE");
	WDeleteFromCList(lst, "TWO");
	WDeleteFromCList(lst, "THREE");

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