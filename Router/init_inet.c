#include <router_defs.h>

#define ROUTER_TOKEN	"Router:"

int32_t readInitFile(char* fName, struct WLList* inetList)
{
	struct Router* Rtr;
	char lineBuf[1024];
	FILE* fp;
	char A[16+1], M[3+1], R[16+1];
	int32_t I;
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
			Rtr = (struct Router*)malloc(sizeof * Rtr);
			memset(Rtr, 0x0, sizeof * Rtr);
			Rtr->Id = atoi(lineBuf + strlen(ROUTER_TOKEN));
		}
		else {
			sscanf(lineBuf, "%s %s %s %d", A, M, R, &I);
			A[strlen(A) - 1] = NUL;
			M[strlen(M) - 1] = NUL;
			R[strlen(R) - 1] = NUL;
			addRoute2RoutingTable(Rtr->FwdgTbl, A, M, R, I);
		}
	}
	if (Rtr) {
		WAppendToList(inetList, (void*)Rtr);
		free(Rtr); Rtr = NULL;
	}
	fclose(fp);
	return 0;
}