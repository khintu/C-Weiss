#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);
void unit_test_network_lib(void);

/* 
	In our implementation the Router Interface Id is a unique
	Autonomous System (AS) Id across the internet/graph, starting
	at 1 and at most MAX_GRAPH_VERTICES. Do NOT interpret interface
	Id as the hardware physical address or MAC.
	We'll have Graph Algorithms from Wikipedia in this program.
*/

/* Actual memory address of AS(s) from FwdgTbl of each router */
struct Router* gIntfTbl[MAX_INTFTBL_SIZE];

void printDistance(struct GVertex* v)
{
	printf("AS:%d @ Distance:%g\n", v->router->Id, v->distance);
	return;
}

int main(int argc, char* argv[])
{
	struct WLList* inetList, *grphList;
	//unit_test_network_lib();
	//insert_in_main_unittestsuit(argc, argv);
	//return 0;
	printf("*** Internet Routing Algorithms Simulator ***\n\n");
	inetList = initializeInternetMap();
	grphList = initializeGraphContainer(inetList);
	printf("Single Source Shortest Path:\n");
	graphDijikstraCalcDistance2(grphList, inetList, 1);
	WIteratorList(grphList, (void (*)(void*))printDistance);
	//resetEverythingInIntrnt(inetList);
	
	WDeleteList(grphList);
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