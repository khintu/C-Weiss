#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);
void unit_test_network_lib(void);


int main(int argc, char* argv[])
{
	struct RouteEntry* FwdgTbl[MAX_FWDGTBL_ENTRIES] = { 0 };
	uint32_t nextHopIP, nextHopIntf;

	//unit_test_network_lib();
	//insert_in_main_unittestsuit(argc, argv);
	addRoute2RoutingTable(FwdgTbl, "128.0.0.0", "8", "128.170.200.210", 3);
	addRoute2RoutingTable(FwdgTbl, "128.170.0.0", "16", "128.170.201.206", 5);
	addRoute2RoutingTable(FwdgTbl, "128.170.189.0", "24", "128.170.201.201", 4);
	addRoute2RoutingTable(FwdgTbl, "0.0.0.0", "0", "128.170.202.255", 6);
	printRoutingTable(FwdgTbl);
	if ((nextHopIP = getNextHopFrmRoutingTable(FwdgTbl, "128.170.189.11")) && \
		  (nextHopIntf = getInterfaceFrmRoutingTable(FwdgTbl, "128.170.189.11")))
	{
		printf("Route for %s is NextHop %s, Intf %d\n", \
					 "128.170.189.11", decimal2dotted32(nextHopIP), nextHopIntf);
	}
	//removeRouteFrmRoutingTable(FwdgTbl, "128.170.188.255", "16");
	//removeRouteFrmRoutingTable(FwdgTbl, "128.170.187.255", "8");
	//printRoutingTable(FwdgTbl);
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