#include <router_defs.h>
#include <randomized.h>

extern int insert_in_main_algolib_unittestsuit(int argc, char* argv[]);
void unit_test_network_lib(void);
void runDijkstrasOSPFAlgo(struct WLList* inetList);
void runConnectedComponentsAlgo(struct WLList* inetList);
void runConnectedComponents2Algo(struct WLList* inetList);
void runKruskalsMSTAlgo(struct WLList* inetList);
void runPrimsMSTAlgo(struct WLList* inetList);
void runBellmanFordAlgo(struct WLList* inetList);
void testSkipList(void);

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
	//insert_in_main_algolib_unittestsuit(argc, argv);

	printf("*** Internet Routing Algorithms Simulator ***\n\n");
	inetList = initializeInternetMap();
	
	// Put your code here
	//printf("Single Source Shortest Path:\n");
	//runBellmanFordAlgo(inetList);
	testSkipList();

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
	grphList = initializeDjGraphContainer(inetList);
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

/* Bellman-Ford single source shortest path */
void runBellmanFordAlgo(struct WLList* inetList)
{
	struct BFGraph* G;
	G = initializeBFGraphContainer(inetList);
	graphBellmnFrdCalcDistance(G, 1);
	graphPrintSrc2AllVtxPaths(G, 1);
	DeleteBFGraph(G);
	return;
}

/* --- Misc Algorithms --- */

void* WAppendToList2(struct WLList* l, void* data)
{
	struct LNode* tmp;
	if ((tmp = (struct LNode*)calloc(1, sizeof(struct LNode))) == NULL)
		return NULL;
	tmp->data = (*l->CTOR)(data);
	if (l->head == NULL)
		l->head = tmp;
	if (l->tail != NULL)
		l->tail->next = tmp;
	l->tail = tmp;
	l->count++;
	return tmp->data;
}

/* Self join with input list */
void WIteratorList2(struct WLList* l, void (*ITR)(void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, l);
	return;
}

void WIteratorList2Arg(struct WLList* l, void* arg, void (*ITR)(void*, void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, l, arg);
	return;
}

void WIteratorList3(struct WLList* l, void* arg, void (*ITR)(void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, arg);
	return;
}

void WIteratorList4(struct WLList* l, void* arg1, void* arg2, void (*ITR)(void*, void*, void*))
{
	struct LNode* p;

	for (p = l->head; p; p = p->next)
		(*ITR)(p->data, arg1, arg2);
	return;
}

void* WGetNthData(struct WLList* l, uint32_t n)
{
	struct LNode* p;
	uint32_t i;
	for (i = 0, p = l->head; p && i < n; p = p->next, i++)
		;
	return p ? p->data : NULL;
}

/* Skip List Test function */

static void* strCtor(void* data)
{
	return (data == NULL) ? NULL : _strdup((char*)data);
}

static void strDtor(void* data)
{
	if (data)
		free(data);
	return;
}

void testSkipList(void)
{
	struct WSkipList* sklist;
	int i;
	sklist = WCreateSkipList(0.5, 10, (WCTRFP)strCtor, (WDTRFP)strDtor, (WCMPFP)strcmp);
	for (i = 0; i < 10; i++) {
		char* str = (char*)calloc(1, 10);
		sprintf(str, "%d", i);
		WInsertSkipList(sklist, str);
		free(str);
	}
	for (i = 0; i < 10; i++) {
		char* str = (char*)calloc(1, 10);
		sprintf(str, "%d", i);
		if (WSearchSkipList(sklist, str) != NULL)
			printf("Found %s\n", str);
		free(str);
	}
	for (i = 0; i < 10; i++) {
		char* str = (char*)calloc(1, 10);
		sprintf(str, "%d", i);
		printf("Deleted key %s = %d\n", str, WDeleteFrmSkipList(sklist, str));
		free(str);
	}
	WDeleteSkipList(sklist);
	return;
}