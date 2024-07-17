#include <router_defs.h>

#define ROUTER_TOKEN	"Router:"


int RouterCmp(const struct Router* R1, const struct Router* R2)
{
	if (R1->Id > R2->Id)
		return 1;
	else if (R1->Id < R2->Id)
		return -1;
	else
		return 0;
}

struct Router* RouterCtor(struct Router* x)
{
	struct Router* R;
	if (R = (struct Router*)malloc(sizeof * x))
		memcpy(R, x, sizeof * x);
	return R;
}

void RouterDtor(struct Router* x)
{
	freeRoutingTable(x->FwdgTbl);
	free(x);
	return;
}

void PrintRoutgTbls(struct Router* R)
{
	printf("RouterId: %d\n", R->Id);
	printRoutingTable(R->FwdgTbl);
}

int32_t readInitFile(char* fName, struct WLList* inetList)
{
	struct Router* Rtr;
	char lineBuf[1024];
	FILE* fp;
	char A[16+1], M[3+1], R[16+1];
	int32_t I;
	float Metric;
	if (!(fp = fopen(fName, "r")))
		return -1;
	Rtr = NULL;
	while (fgets(lineBuf, sizeof lineBuf, fp)) {
		if (lineBuf[0] == '\n') {
			if (Rtr) {
				WAppendToList(inetList, (void*)Rtr);
				free(Rtr); Rtr = NULL;
			}
			continue;
		}
		else if (!strncmp(lineBuf, ROUTER_TOKEN, strlen(ROUTER_TOKEN))) {
			if (Rtr = (struct Router*)malloc(sizeof * Rtr)) {
				memset(Rtr, 0x0, sizeof * Rtr);
				Rtr->Id = atoi(lineBuf + strlen(ROUTER_TOKEN));
			}
		}
		else {
			sscanf(lineBuf, "%s %s %s %d %f", A, M, R, &I, &Metric);
			A[strlen(A) - 1] = NUL;
			M[strlen(M) - 1] = NUL;
			R[strlen(R) - 1] = NUL;
			addRoute2RoutingTable(Rtr->FwdgTbl, A, M, R, I, Metric);
		}
	}
	if (Rtr) {
		WAppendToList(inetList, (void*)Rtr);
		free(Rtr); Rtr = NULL;
	}
	fclose(fp);
	return 0;
}

struct WLList* initializeInternetMap(void)
{
	struct WLList* inetList;
	inetList = WCreateList((WCMPFP)RouterCmp, (WCTRFP)RouterCtor, (WDTRFP)RouterDtor);
	readInitFile("route_init.ini", inetList);
	WIteratorList(inetList, (void (*)(void*))PrintRoutgTbls);
	generateLinksBwRouters(inetList);
	return inetList;
}

void RouterGenLinks(struct Router* Rtr, struct WLList* inetList)
{
	int32_t i;
	struct Router* x, tmp = { 0 };
	
	printf("Interface link associations for %d\n", Rtr->Id);

	for (i = 0; Rtr->FwdgTbl[i] && i < MAX_FWDGTBL_ENTRIES; ++i) {
		tmp.Id = Rtr->FwdgTbl[i]->I;
		if (x = WFindInList(inetList, &tmp)) {
			if (x->Id < MAX_INTFTBL_SIZE) {
				Rtr->IntfTbl[x->Id] = x;
				printf("b/w % d and %d\n", Rtr->FwdgTbl[i]->I, x->Id);
			}
		}
	}
	return;
}

void generateLinksBwRouters(struct WLList* inetList)
{
	WIteratorList2(inetList, (void (*)(void*, void*))RouterGenLinks);
	return;
}

void resetAdPurgeEntsFrmRouter(struct Router* Rtr)
{
	freeRoutingTable(Rtr->FwdgTbl);
	memset(Rtr->IntfTbl, 0x0, sizeof Rtr->IntfTbl);
	return;
}