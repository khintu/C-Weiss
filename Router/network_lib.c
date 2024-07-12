#include <router_defs.h>


uint32_t reversebyteorder32(uint32_t h);
uint16_t reverseobyteorder16(uint16_t h);
uint32_t reversebyteorder32Fast(uint32_t h);
uint16_t reversebyteorder16Fast(uint16_t h);


uint32_t reversebyteorder32(uint32_t h)
{
	uint8_t bH[4], bN[4];
	uint32_t n;

	memcpy(bH, &h, sizeof h);
	bN[0] = bH[3]; bN[1] = bH[2]; bN[2] = bH[1]; bN[3] = bH[0];
	memcpy(&n, bN, sizeof n);
	return n;
}

uint16_t reversebyteorder16(uint16_t h)
{
	uint8_t bH[2], bN[2];
	uint16_t n;

	memcpy(bH, &h, sizeof h);
	bN[0] = bH[1]; bN[1] = bH[0];
	memcpy(&n, bN, sizeof n);
	return n;
}

uint32_t reversebyteorder32Fast(uint32_t h)
{
	uint32_t mask = 0x000000FF, n, tmp;
	int32_t i;
	for (tmp = 0, n = 0, i = 0; i < 4; ++i, tmp = 0) {
		tmp |= ((h & mask << (i * 8)) >> (i * 8));
		tmp <<= ((3 - i) * 8);
		n |= tmp;
	}
	return n;
}

uint16_t reversebyteorder16Fast(uint16_t h)
{
	uint16_t mask = 0x00FF, n, tmp;
	int32_t i;
	for (tmp = 0, n = 0, i = 0; i < 2; ++i, tmp = 0) {
		tmp |= ((h & mask << (i * 8)) >> (i * 8));
		tmp <<= ((1 - i) * 8);
		n |= tmp;
	}
	return n;
}

uint32_t host2network32(uint32_t h)
{
	return reversebyteorder32Fast(h);
}
uint32_t network2host32(uint32_t n)
{
	return reversebyteorder32Fast(n);
}
uint16_t host2network16(uint16_t h)
{
	return reversebyteorder16Fast(h);
}
uint16_t network2host16(uint16_t n)
{
	return reversebyteorder16Fast(n);
}

/* Match against Host Specific, Same Network, Intranet/Known networks,
	 Internet/Unknown networks (Defualt Gw), destination(s) */
struct RouteEntry* longestPrefixMatch(struct RouteEntry* FwdgTbl[], uint32_t DestinationDatagram)
{
	int32_t i;
	for (i = 0; FwdgTbl[i] && i < MAX_FWDGTBL_ENTRIES; i++) {
		if ((FwdgTbl[i]->M & DestinationDatagram) == FwdgTbl[i]->A)
			return FwdgTbl[i];
	}
	return NULL;
}

/* Sort in descending order of Network Prefix in Route entry */
int RouteCmp(struct RouteEntry* x, struct RouteEntry* y)
{
	if (x->M > y->M)
		return -1;
	else if (x->M < y->M)
		return 1;
	else
		return 0;
}

void longestPrefixOrdered(struct RouteEntry* FwdgTbl[], int32_t startIdx, int32_t endIdx)
{
	//WQuickSort((void**)FwdgTbl, startIdx, endIdx, (int (*)(void*, void*)) & RouteCmp);
	WMergeSort((void**)FwdgTbl, startIdx, endIdx, (int (*)(const void*, const void*)) & RouteCmp);
	return;
}

int32_t addRoute2FwdTbl(struct RouteEntry* FwdgTbl[], struct RouteEntry* x)
{
	int32_t i;
	for (i = 0; i < MAX_FWDGTBL_ENTRIES; ++i) {
		if (FwdgTbl[i] == NULL) {
			FwdgTbl[i] = x;
			return i;
		}
	}
	return -1;
}

int32_t removeRouteAdPackFwdTbl(struct RouteEntry* FwdgTbl[], struct RouteEntry* x)
{
	int32_t i, j;

	if (FwdgTbl[0] == NULL)
		return -1; /* Route entry not found, table is empty as it stays packed */
	for (i = 0; i < MAX_FWDGTBL_ENTRIES; ++i) {
		if( FwdgTbl[i]->A == x->A && FwdgTbl[i]->M == x->M /*&& \
			FwdgTbl[i]->R == x->R && FwdgTbl[i]->I == x->I*/) {
			free(FwdgTbl[i]); /* Route entry found & deleted */
			FwdgTbl[i] = NULL;
			break;
		}
	}
	if (i == MAX_FWDGTBL_ENTRIES) {
		return -1; /* Route entry not found */
	}
	/* Pack the fwdg table, it is not empty case */
	for (i = 0; i < MAX_FWDGTBL_ENTRIES; ++i) {
		if (FwdgTbl[i] == NULL) {
			for (j = i + 1; j < MAX_FWDGTBL_ENTRIES; ++j) {
				FwdgTbl[j - 1] = FwdgTbl[j];
			}
			break; /* No need to look for more NULL entries, from above */
		}
	}
	return 0;
}

uint32_t dotted2decimal32(const char* d)
{
	uint32_t x, y, z;
	for (x = 0, y = 0, z = 3; *d != NUL ; d++) {
		if (*d != '.') {
			y = 10*y + (*d - '0');
		}
		else {
			x |= y << (z*8);
			y = 0; z--;
		}
	}
	x |= y;
	return x;
}

uint32_t ntwkprefx2decimal32(const char* np)
{
	int32_t np2, i;
	uint32_t x = 0, mask = 1;

	for (np2 = 0; *np != NUL; ++np) {
		np2 = np2 * 10 + (*np - '0');
	}
	if (np2 >= 0 && np2 <= 32) { /* Permitted values between 0 & 32 */
		for (i = 31; i > 0 && np2-- > 0; --i)
			x |= (mask << i);
	}
	return x;
}

char* itoa8(uint8_t d)
{
	int32_t lsb, i, j, len, tmp;
	static char a[4];
	
	memset(a, 0x0, 4);
	for (i = 0, a[0] = '0'; d && i < 3 ; ++i, d /= 10) {
		lsb = d % 10 + '0';
		a[i] = lsb;
	}
	len = (int32_t)strlen(a);
	for (i = len - 1, j = 0; i > (len / 2 - 1) ; --i, ++j) {
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}
	return a;
}

char* decimal2dotted32(uint32_t ipAddr)
{
	static char dottd[16];
	uint8_t a[4] = { 0 };
	int32_t idx;

	memset(dottd, 0x0, 16);
	a[0] = ipAddr >> 24; /* MSB */
	a[1] = (ipAddr << 8) >> 24;
	a[2] = (ipAddr << 16) >> 24;
	a[3] = (ipAddr << 24) >> 24; /* LSB */
	strcpy(dottd, itoa8(a[0]));
	idx = (int32_t)strlen(dottd);
	dottd[idx] = '.';
	
	strcat(dottd, itoa8(a[1]));
	idx = (int32_t)strlen(dottd);
	dottd[idx] = '.';

	strcat(dottd, itoa8(a[2]));
	idx = (int32_t)strlen(dottd);
	dottd[idx] = '.';

	strcat(dottd, itoa8(a[3]));
 	return dottd;
}