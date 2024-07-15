#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);
void unit_test_network_lib(void);

int RouterCmp(const struct Router* R1, const struct Router* R2)
{
	if (R1->Id > R2->Id)
		return 1;
	else if (R1->Id < R1->Id)
		return -1;
	else
		return 0;
}

struct Router* RouterCtor(struct Router* x)
{
	struct Router* R;
	R = (struct Router*)malloc(sizeof * x);
	memcpy(R, x, sizeof * x);
	return R;
}

void RouterDtor(struct Router* x)
{
	freeRoutingTable(x->FwdgTbl);
	free(x);
	return;
}

void RouterItr(struct Router* R)
{
	printf("RouterId: %d\n", R->Id);
	printRoutingTable(R->FwdgTbl);
}

int main(int argc, char* argv[])
{
	struct WLList* inetList;
	//unit_test_network_lib();
	//insert_in_main_unittestsuit(argc, argv);
	
	inetList = WCreateList((WCMPFP)RouterCmp, (WCTRFP)RouterCtor, (WDTRFP)RouterDtor);
	readInitFile("route_init.ini", inetList);
	WIteratorList(inetList, (void (*)(void*))RouterItr);
	

	//removeRouteFrmRoutingTable(FwdgTbl, "128.170.189.0", "24");
	//removeRouteFrmRoutingTable(R1->FwdgTbl, "0.0.0.0", "0");
	//removeRouteFrmRoutingTable(FwdgTbl, "128.0.0.0", "8");
	//printRoutingTable(R1->FwdgTbl);
	WDeleteList(inetList);
	return 0;
}

void unit_test_network_lib(void)
{
	uint32_t dummy32 = 0x04FF12AB; /* decimal 4.255.18.171 */
	uint32_t dummy16 = 0x12AB; /* decimal 4,779 */
	printf("%x to %x\n", dummy32, host2network32(dummy32));
	printf("%x to %x\n", host2network32(dummy32), network2host32(host2network32(dummy32)));
	printf("%x to %x\n", dummy16, host2network16(dummy16));
	printf("%x to %x\n", host2network16(dummy16), network2host16(host2network16(dummy16)));
	printf("%s\n", decimal2dotted32(dummy32));
	printf("%s\n", decimal2dotted32(0));
	return;
}