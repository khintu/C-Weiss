#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);
uint32_t host2network32(uint32_t h);
uint16_t host2network16(uint16_t h);
uint32_t host2network32Fast(uint32_t h);
uint16_t host2network16Fast(uint16_t h);

int main(int argc, char* argv[])
{
	uint32_t dummy32 = 0x34FF12AB; /* decimal 52 */
	uint32_t dummy16 = 0x12AB; /* decimal 52 */
	printf("%x to %x\n", dummy32, host2network32(dummy32));
	printf("%x to %x\n", dummy32, host2network32Fast(dummy32));
	printf("%x to %x\n", dummy16, host2network16(dummy16));
	printf("%x to %x\n", dummy16, host2network16Fast(dummy16));
	//insert_in_main_unittestsuit(argc, argv);
	return 0;
}

uint32_t host2network32(uint32_t h)
{
	uint8_t bH[4], bN[4];
	uint32_t n;
	
	memcpy(bH, &h, sizeof h);
	bN[0] = bH[3]; bN[1] = bH[2]; bN[2] = bH[1]; bN[3] = bH[0];
	memcpy(&n, bN, sizeof n);
	return n;
}

uint16_t host2network16(uint16_t h)
{
	uint8_t bH[2], bN[2];
	uint16_t n;

	memcpy(bH, &h, sizeof h);
	bN[0] = bH[1]; bN[1] = bH[0];
	memcpy(&n, bN, sizeof n);
	return n;
}

uint32_t host2network32Fast(uint32_t h)
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

uint16_t host2network16Fast(uint16_t h)
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