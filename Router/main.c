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


int main(int argc, char* argv[])
{
	printf("Router: Hello World\n");
	struct WSet* a;
	a = WCreateSet((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WDeleteSet(a);
	return 0;
}