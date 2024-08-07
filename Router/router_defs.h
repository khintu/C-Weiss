#ifndef ROUTER_DEFS_H
#define ROUTER_DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <c-algorithm.h>

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

/* Graph Algorithms (Dijkstra, Shortest paths, etc.) */
struct GVertex {
	uint32_t unvisited; /* boolean flag */
	float distance;    /* distance from source vertex */
	struct Router* router; /* Router object ref. encapsulated */
	struct GVertex* prev; /* previous vertex on shortest path trace to source */
};

struct WLList* initializeGraphContainer(struct WLList*);
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
void DJSCollectionDestroy(struct DJSCollection*);
struct DJSet* DJSMakeSet(struct DJSCollection*, void*);
void* DJSFindSet(void*);
void DJSUnion(struct DJSCollection**, struct DJSet*, struct DJSet*);

/* Minimum Spanning Tree, using Disjoint Sets */

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

#endif // ROUTER_DEFS_H