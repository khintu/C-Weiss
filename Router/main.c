#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <router_defs.h>

static char* ctor(char* s)
{
	char* p;
	if ((p = (char*)malloc(strlen(s) + 1)) != NULL)
		strcpy(p, s);
	return p;
}

static void dtor(char* s)
{
	free(s);
	return;
}

static void itr(char* s)
{
	printf("%s ", s);
	return;
}

int main(int argc, char* argv[])
{
	printf("Router: Hello World\n");
	struct WSet* a, *b, *c;
	a = WCreateSet((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	b = WCreateSet((WCMPFP)strcmp, (WCTRFP)ctor, (WDTRFP)dtor);
	WInsertKeySet(a, "Where");
	WInsertKeySet(a, "is");
	WInsertKeySet(a, "Tom");
	WUnionOfSet(a, b, &c);
	WIteratorSet(c, (void (*)(void*))itr);
	putchar('\n');
	WDeleteSet(a);
	WDeleteSet(b);
	WDeleteSet(c);
	return 0;
}