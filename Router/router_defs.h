#ifndef ROUTER_DEFS_H
#define ROUTER_DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <c-algorithm.h>

#define MAX_ARRAY_SIZE	128

#define MAX_FWDGTBL_ENTRIES	MAX_ARRAY_SIZE
#define MAX_INTFTBL_SIZE	MAX_ARRAY_SIZE

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
	uint32_t Id;
	struct RouteEntry* FwdgTbl[MAX_FWDGTBL_ENTRIES];
	struct Router* IntfTbl[MAX_INTFTBL_SIZE];
};

int32_t readInitFile(char*, struct WLList*);
struct WLList* initializeInternetMap(void);
void generateLinksBwRouters(struct WLList*);
void resetAdPurgeEntsFrmRouter(struct Router*);

#endif // ROUTER_DEFS_H