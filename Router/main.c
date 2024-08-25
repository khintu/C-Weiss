#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);
void unit_test_network_lib(void);
void runDijkstrasOSPFAlgo(struct WLList* inetList);
void runConnectedComponentsAlgo(struct WLList* inetList);
void runConnectedComponents2Algo(struct WLList* inetList);
void runKruskalsMSTAlgo(struct WLList* inetList);
void runPrimsMSTAlgo(struct WLList* inetList);

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
	struct WLList* inetList;
	//insert_in_main_unittestsuit(argc, argv);

	printf("*** Internet Routing Algorithms Simulator ***\n\n");
	inetList = initializeInternetMap();
	
	// Put your code here
	runPrimsMSTAlgo(inetList);

	// Delete internet graph
	resetEverythingInIntrnt(inetList);
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

/* OSPF using Dijkstras algo on Directed weighted Graph */
void runDijkstrasOSPFAlgo(struct WLList *inetList)
{
	struct WLList *grphList;
	grphList = initializeGraphContainer(inetList);
	printf("Single Source Shortest Path:\n");
	graphDijikstraCalcDistance2(grphList, inetList, 1);
	WIteratorList(grphList, (void (*)(void*))printDistance);
	graphTraceShortstPathFrmSrc2Trgt(grphList, inetList, 7);
	WDeleteList(grphList);
	return;
}

/* Connected Components, using disjoint sets on Undirected weighted graph */
void runConnectedComponentsAlgo(struct WLList* inetList)
{
	struct MSTGraph* G;
	struct DJSCollection* S;
	S = DJSCreateCollection();
	G = initializeMSTGraphContainer(inetList);
	ConnectedComponentsGraph(G, S);
	printConnectedComponents(G);
	DJSDestroyCollection(S);
	DeleteMSTGraph(G);
	return;
}

/* Connected Components, using Rooted disjoint sets on Undirected weighted graph */
void runConnectedComponents2Algo(struct WLList* inetList)
{
	struct MST2Graph* G;
	struct DJSRtCollctn* S = NULL;
	G = initializeMST2GraphContainer(inetList);
	ConnectedComponentsGraph2(G, &S);
	printConnectedComponents2(G);
	DJSRtDestroyCollctn(S, G);
	DeleteMST2Graph(G);
	return;
}

/* Kruskals MST, using Rooted disjoint sets on Undirected weighted graph */
void runKruskalsMSTAlgo(struct WLList* inetList)
{
	struct WLList* A = NULL;
	struct MST2Graph* G;
	struct DJSRtCollctn* S = NULL;

	G = initializeMST2GraphContainer2(inetList);
	InitializeVerticsFrMST(G, &S, &A);
	FindSafeEdgesAdBuildMST(G, &S, A);
	printSafeEdgesMST(A);
	DJSRtDestroyCollctn(S, G);
	WDeleteList(A);
	DeleteMST2Graph(G);
	return;
}

/* Prims MST using  min binanry heap */
void runPrimsMSTAlgo(struct WLList* inetList)
{
	struct WLList* G;

	G = initializePMSTGraphContainer(inetList);
	FindLightEdgesOnGraphCuts(1, G);
	DeletePMSTGraph(G);
	return;
}