#include <router_defs.h>

void printRoutingTable(struct RouteEntry const * FwdgTbl[])
{
	int32_t i;
	printf("Routing/Forwarding Table:\n");
	for (i = 0; FwdgTbl[i] != NULL; ++i)
		printf("%.8x, %.8x, %.8x, %d\n", FwdgTbl[i]->A, FwdgTbl[i]->M, FwdgTbl[i]->R, FwdgTbl[i]->I);
	return;
}

int32_t addRoute2RoutingTable(struct RouteEntry* FwdgTbl[],
															const char* IPAddr,
															const char* NetPrefx,
															const char* NextHop,
															uint32_t IntfId)
{
	struct RouteEntry* route;
	int32_t idx;
	if ((route = (struct RouteEntry*)malloc(sizeof * route)) == NULL)
		return -1;
	route->A = dotted2decimal32(IPAddr);
	route->M = ntwkprefx2decimal32(NetPrefx);
	route->R = dotted2decimal32(NextHop);
	route->I = IntfId;
	idx = addRoute2FwdTbl(FwdgTbl, route);
	
	if (idx > 0) {
		printf("Route Added %x, %x, %x, %d\n", FwdgTbl[idx]->A, FwdgTbl[idx]->M, \
			FwdgTbl[idx]->R, FwdgTbl[idx]->I);
		longestPrefixOrdered(FwdgTbl, 0, idx);
	}
	return 0;
}

void removeRouteFrmRoutingTable(struct RouteEntry* FwdgTbl[], const char* IPAddr, const char* NetPrefx)
{
	struct RouteEntry route = { 0 };

	route.A = dotted2decimal32(IPAddr);
	route.M = ntwkprefx2decimal32(NetPrefx);
	if (removeRouteAdPackFwdTbl(FwdgTbl, &route) == 0)
		printf("Route Removed %.8x, %.8x\n", route.A, route.M);

	return;
}

/* IP addresses 0.0.0.0 & 255.255.255.255 are reserved and not part of addressable IPs */
uint32_t getNextHopFrmRoutingTable(struct RouteEntry* FwdgTbl[], const char *DestinationIP)
{
	struct RouteEntry* nextHop;

	nextHop = longestPrefixMatch(FwdgTbl, dotted2decimal32(DestinationIP));

	return (nextHop)? nextHop->R : 0;
}

/* zero is an invalid interface ID */
uint32_t getInterfaceFrmRoutingTable(struct RouteEntry* FwdgTbl[], const char *DestinationIP)
{
	struct RouteEntry* nextHop;

	nextHop = longestPrefixMatch(FwdgTbl, dotted2decimal32(DestinationIP));

	return (nextHop) ? nextHop->I : 0;
}