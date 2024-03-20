#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
}

int UnitTestWLList(int argc, char* argv[])
{
	struct WLList* lst;

	printf("Unit Test Linked List\n");

	lst = WCreateList(strcmp, ctor, dtor);
	WAddToList(lst, "UPTRON");
	WAddToList(lst, "UPFC");
	WPrependToList(lst, "LUCKNOW");
	WAppendToList(lst, "NEW DELHI");
	WInsertToList(lst, "UPTRON", "SFC");
	WDeleteFromList(lst, "SFC");
	printf("Find:%s\n", (char*)WFindInList(lst, "SFC"));
	WIteratorList(lst, iterator);
	WDeleteList(lst);
	return 0;
}

int UnitTestWDLList(int argc, char* argv[])
{
	struct WDLList* lst;

	printf("Unit Test Doubly Linked List\n");

	lst = WCreateDList(strcmp, ctor, dtor);
	WAddToDList(lst, "UPTRON");
	WAddToDList(lst, "UPFC");
	WPrependToDList(lst, "LUCKNOW");
	WAppendToDList(lst, "NEW DELHI");
	WInsertToDList(lst, "UPTRON", "SFC");
	WInsertToDList(lst, "LUCKNOW", "BIT");
	WInsertToDList(lst, "NEW DELHI", "USC");
	WDeleteFromDList(lst, "LUCKNOW");
	printf("Find:%s\n", (char*)WFindInDList(lst, "SFC"));
	WRevIteratorDList(lst, iterator);
	WDeleteDList(lst);
	return 0;
}

int main(int argc, char* argv[])
{
	printf("Hello Weiss!!\n");
	
	UnitTestWLList(argc, argv);
	UnitTestWDLList(argc, argv);

	return 0;
}