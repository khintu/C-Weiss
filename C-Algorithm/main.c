#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <c-algorithm.h>

char* ctor(char* s)
{
	char* p;
	if ((p = (char*)calloc(strlen(s) + 1, sizeof(char))) != NULL)
		strcpy(p, s);
	return p;
}

void dtor(char* s)
{
	free(s);
	return;
}


int main(int argc, char* argv[])
{
	printf("Hello Weiss!!\n");
	
	struct WLList* lst;

	lst = WCreateList(strcmp, ctor, dtor);
	WAddToList(lst, "UPTRON");
	WAddToList(lst, "UPFC");
	WFindInList(lst, "UPFC");
	WDeleteList(lst);
	return 0;
}