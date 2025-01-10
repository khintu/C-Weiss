#ifndef ROUTER_DEFS_H
#define ROUTER_DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <c-algorithm.h>

/* Floating point overflow checking */
#include <math.h>
#include <errno.h>
#include <fenv.h>

#define MAX_GRAPH_VERTICES	128

#define MAX_FWDGTBL_ENTRIES	MAX_GRAPH_VERTICES
#define MAX_INTFTBL_SIZE	MAX_GRAPH_VERTICES

/* Global AS/Interface Id table */
extern struct Router* gIntfTbl[MAX_INTFTBL_SIZE];

/* IPv4 Routing Table entry, APIs */
struct RouteEntry
{
	uint32_t A;
	uint32_t M;
	uint32_t R;
	uint32_t I;
	float Metric;
};

/* Forwarding algorithms */
void longestPrefixOrdered(struct RouteEntry* [], int32_t, int32_t);
struct RouteEntry* longestPrefixMatch(struct RouteEntry* [], uint32_t);
int32_t addRoute2FwdTbl(struct RouteEntry* [], struct RouteEntry*);
int32_t removeRouteAdPackFwdTbl(struct RouteEntry* [], struct RouteEntry*);

/* Routing Table Interface APIs */
void printRoutingTable(struct RouteEntry const* []);
int32_t addRoute2RoutingTable(struct RouteEntry* [], const char*, const char*, const char*, uint32_t, float);
void removeRouteFrmRoutingTable(struct RouteEntry* [], const char*, const char*);
uint32_t getNextHopFrmRoutingTable(struct RouteEntry* [], const char*);
uint32_t getInterfaceFrmRoutingTable(struct RouteEntry* [], const char*);
uint32_t getNextHopFrmRoutingTable2(struct Router*, const char*);
void freeRoutingTable(struct RouteEntry* []);

/* Helper Network APIs */
uint32_t host2network32(uint32_t);
uint32_t network2host32(uint32_t);
uint16_t host2network16(uint16_t);
uint16_t network2host16(uint16_t);

uint32_t dotted2decimal32(const char*);
uint32_t ntwkprefx2decimal32(const char*);
char* decimal2dotted32(uint32_t);
char* itoa8(uint8_t);

/* Internet/WAN Builder APIs */
struct Router {
	uint32_t Id; /* Unique AS id in internet/graph */
	struct RouteEntry* FwdgTbl[MAX_FWDGTBL_ENTRIES];
	void* super;	/* For a parent struct that encapsulates a Router */
};

int32_t readInitFile(char*, struct WLList*);
struct WLList* initializeInternetMap(void);
void generateLinksBwRouters(struct WLList*);
void resetAdPurgeEntsFrmRouter(struct Router*);
void resetEverythingInIntrnt(struct WLList*);

/* Misc Algorithms */
void* WAppendToList2(struct WLList* l, void* data);

/* Graph Algorithms (Dijkstra, Shortest paths, etc.) */
struct GVertex {
	uint32_t unvisited; /* boolean flag */
	float distance;    /* distance from source vertex */
	struct Router* router; /* Router object ref. encapsulated */
	struct GVertex* prev; /* previous vertex on shortest path trace to source */
};

struct WLList* initializeDjGraphContainer(struct WLList*);
void graphDijikstraCalcDistance(struct WLList*, struct WLList*, uint32_t);
void graphTraceShortstPathFrmSrc2Trgt(struct WLList*, struct WLList*, uint32_t);
void graphDijikstraCalcDistance2(struct WLList*, struct WLList*, uint32_t);

/* Disjoint Sets, using Linked List, Amortized weigthed-union Heuristic O(m + nlgn) */
struct DJSetNode {
	void* object;
	struct DJSetNode* next;
	struct DJSet* set;
};

struct DJSet {
	struct DJSetNode* head, *tail;
	uint32_t length;
};

struct DJSCollection {
	struct DJSet* S;
	struct DJSCollection* next;
};

struct DJSCollection* DJSCreateCollection(void);
void DJSDestroyCollection(struct DJSCollection*);
struct DJSet* DJSMakeSet(struct DJSCollection*, void*);
void* DJSFindSet(void*);
void DJSUnion(struct DJSCollection**, struct DJSet*, struct DJSet*);
struct DJSet* FindSetInCollctn(struct DJSCollection*, struct MSTVertex*);

/* Minimum Spanning Tree, using Disjoint Sets LL */
struct MSTVertex {
	uint32_t vrtxId;
	struct DJSetNode* setNode;
};

struct MSTEdge {
	struct MSTVertex* u, * v;
	float weigth;
};

struct MSTGraph {
	struct WLList* vertices;
	struct WLList* edges;
};

struct MSTGraph* initializeMSTGraphContainer(struct WLList*);
void DeleteMSTGraph(struct MSTGraph*);
void ConnectedComponentsGraph(struct MSTGraph*, struct DJSCollection*);
int32_t isSameConnectedComponent(struct MSTVertex*, struct MSTVertex*);
void printConnectedComponents(struct MSTGraph*);

/* Disjoint Sets, using Rooted Trees, Amortized 2*Heuristics O(m), super linear */
struct DJSRtNode {
	uint32_t rank;				/* height from root to leaf of a node for union-by-rank */
	struct DJSRtNode* p; /* parent node for path-compression */
	struct MST2Vertex* vertex; /* reference to Graph */
};

struct DJSRtCollctn {
	struct DJSRtNode* root;	/* root node or representative of DJSRt */
	struct DJSRtCollctn* next;
};

void DJSRtAdd2Collctn(struct DJSRtCollctn**, struct DJSRtNode*);
void DJSRtDestroyCollctn(struct DJSRtCollctn*, struct MST2Graph*);
void DJSRtMakeSet(struct DJSRtNode*, struct MST2Vertex*);
struct DJSRtNode* DJSRtFindSet(struct DJSRtNode*);
void DJSRtUnion(struct DJSRtCollctn**, struct DJSRtNode*, struct DJSRtNode*);

/* Minimum Spanning Tree/Forest, using DJS-Rooted Trees */
struct MST2Vertex {
	uint32_t vrtxId;
	struct DJSRtNode* setNode;	/* reference to DJSets */
};

struct MST2Edge {
	struct MST2Vertex* u, * v;
	float weigth;
};

struct MST2Graph {
	struct WLList* vertices;
	struct WLList* edges;
};

/* Connected Components algo */
struct MST2Graph* initializeMST2GraphContainer(struct WLList*);
void DeleteMST2Graph(struct MST2Graph*);
void ConnectedComponentsGraph2(struct MST2Graph*, struct DJSRtCollctn**);
int32_t isSameConnectedComponent2(struct MST2Vertex*, struct MST2Vertex*);
void printConnectedComponents2(struct MST2Graph*);

/* Kruskals Minimum Spanning Forest algo */
struct MST2Graph* initializeMST2GraphContainer2(struct WLList*);
void InitializeVerticsFrMST(struct MST2Graph*, struct DJSRtCollctn**, struct WLList**);
void FindSafeEdgesAdBuildMST(struct MST2Graph*, struct DJSRtCollctn**, struct WLList*);
void printSafeEdgesMST(struct WLList*);

/* Prims Minimum Spanning Tree algo */
struct PMSTVertex {
	uint32_t vrtxId; /* Vertex/Router Id */
	float key; /* minimum edge over a cut connecting to a parent tree(A) vertex */
	struct PMSTVertex* p; /* reference to parent vertex */
	struct WLList* Adj; /* Edges u(this)-v */
};

struct PMSTEdge {
	float weigth;
	struct PMSTVertex* v; /* Reference to adjacent vertex v of vertex u */
};

struct WLList* initializePMSTGraphContainer(struct WLList*);
void DeletePMSTGraph(struct WLList *);
void FindLightEdgesOnGraphCuts(uint32_t idx, struct WLList*);

/* Bellman-Ford shortest path graph algorithm, with -ve edge cycle, (RIP) */
struct BFVertex {
	uint32_t vrtxId;
	float distance;
	struct Router* router;
	struct BFVertex* pred;
};

struct BFEdge {
	float weigth;
	struct BFVertex	*u, *v;
};

struct BFGraph {
	struct WLList* vertices;
	struct WLList* edges;
};

struct BFGraph* initializeBFGraphContainer(struct WLList*);
void DeleteBFGraph(struct BFGraph*);

#endif // ROUTER_DEFS_H