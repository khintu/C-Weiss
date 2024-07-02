#ifndef ROUTER_DEFS_H
#define ROUTER_DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <c-algorithm.h>

uint32_t host2network32(uint32_t h);
uint32_t network2host32(uint32_t n);
uint16_t host2network16(uint16_t h);
uint16_t network2host16(uint16_t n);



#endif // ROUTER_DEFS_H