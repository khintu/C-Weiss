#include <router_defs.h>

struct Router
{
	char* name;
	int key;
	int nInterfaces;
};

static int RouterCmp(const struct Router* x, const struct Router* y)
{
	if (x->key == y->key)
		return 0;
	else if (x->key > y->key)
		return 1;
	else
		return -1;
}

static struct Router* ctor(struct Router* r)
{
	struct Router* p;

	if ((p = (struct Router*)calloc(1, sizeof * p)) == NULL)
		return NULL;
	if ((p->name = (char*)malloc(strlen(r->name) + 1)) != NULL)
		strcpy(p->name, r->name);
	p->nInterfaces = r->nInterfaces;
	p->key = r->key;
	return p;
}

static void dtor(struct Router* r)
{
	free(r->name);
	free(r);
	return;
}

static struct Router* ctorR(struct Router* r)
{
	return r;
}

static void dtorR(struct Router* r)
{
	return;
}

static void itr(struct Router* r)
{
	printf(" %s, ", r->name);
	return;
}

static void itr2(struct Router* r, int d)
{
	printf(" %s(%d), ", r->name, d);
	return;
}

int main(int argc, char* argv[])
{
	printf("Router: Internet Simulator\n");
#if 1
	struct Router input, input2;
	struct WSet* RouterSet;
	struct WGraph* Internet;
	RouterSet = WCreateSet((WCMPFP)RouterCmp, (WCTRFP)ctor, (WDTRFP)dtor);
	//Internet = WCreateGraph((WCMPFP)RouterCmp, (WCTRFP)ctorR, (WDTRFP)dtorR);
	input.key = 10; input.name = "192.168.254.10"; input.nInterfaces = 3;
	WInsertKeySet(RouterSet, &input);
	input.key = 11; input.name = "192.168.254.11"; input.nInterfaces = 4;
	WInsertKeySet(RouterSet, &input);
	input.key = 12; input.name = "192.168.254.12"; input.nInterfaces = 3;
	WInsertKeySet(RouterSet, &input);
	WIteratorSet(RouterSet, (void (*)(void*))itr);
	putchar('\n');
	WConvertSetToGraph2(RouterSet, &Internet, (WCTRFP)ctorR, (WDTRFP)dtorR);
	input.key = 10; input.name = "192.168.254.10"; input.nInterfaces = 3;
	input2.key = 11; input2.name = "192.168.254.11"; input2.nInterfaces = 4;
	WAddEdgeToGraph(Internet, &input, &input2);
	input.key = 11; input.name = "192.168.254.11"; input.nInterfaces = 4;
	input2.key = 12; input2.name = "192.168.254.12"; input2.nInterfaces = 3;
	WAddEdgeToGraph(Internet, &input, &input2);
	input.key = 10; input.name = "192.168.254.10"; input.nInterfaces = 3;
	WBreadthFirstSearchGraph(Internet, &input, (void (*)(void*,int))itr2);
	putchar('\n');
	WDeleteGraph(Internet);
	WDeleteSet(RouterSet);
#else
	UnitTestWFibHeap(argc, argv);
#endif
	return 0;
}