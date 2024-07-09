#include <router_defs.h>

void printRoutingTable(struct RouteEntry const * FwdgTbl[])
{
	int32_t i;
	printf("Routing/Forwarding Table:\n");
	for (i = 0; FwdgTbl[i] != NULL; ++i)
		printf("%x, %x, %x, %d\n", FwdgTbl[i]->A, FwdgTbl[i]->M, FwdgTbl[i]->R, FwdgTbl[i]->I);
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
	printf("Route Added %x, %x, %x, %d\n", FwdgTbl[idx]->A, FwdgTbl[idx]->M,\
				 FwdgTbl[idx]->R, FwdgTbl[idx]->I);
	if (idx > 0)
		longestPrefixOrdered(FwdgTbl, 0, idx);
	return 0;
}

void removeRouteFrmRoutingTable(struct RouteEntry* FwdgTbl[], const char* IPAddr, const char* NetPrefx)
{
	struct RouteEntry route;

	route.A = dotted2decimal32(IPAddr);
	route.M = ntwkprefx2decimal32(NetPrefx);
	removeRouteAdPackFwdTbl(FwdgTbl, &route);

	return;
}