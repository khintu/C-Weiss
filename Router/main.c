#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);


struct RouteEntry* FwdgTbl[MAX_FWDGTBL_ENTRIES];




int main(int argc, char* argv[])
{
	uint32_t dummy32 = 0x34FF12AB; /* decimal 88,91,31,691 */
	uint32_t dummy16 = 0x12AB; /* decimal 4,779 */
	printf("%x to %x\n", dummy32, host2network32(dummy32));
	printf("%x to %x\n", host2network32(dummy32), network2host32(host2network32(dummy32)));
	printf("%x to %x\n", dummy16, host2network16(dummy16));
	printf("%x to %x\n", host2network16(dummy16), network2host16(host2network16(dummy16)));
	//insert_in_main_unittestsuit(argc, argv);
	
	addRoute2RoutingTable(FwdgTbl, "128.170.187.255", "8", "128.170.200.255", 3);
	addRoute2RoutingTable(FwdgTbl, "128.170.188.255", "16", "128.170.201.255", 4);
	printRoutingTable(FwdgTbl);
	removeRouteFrmTable(FwdgTbl, "128.170.188.255", "16");
	printRoutingTable(FwdgTbl);
	return 0;
}