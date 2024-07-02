#include<router_defs.h>

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