#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	uint32_t dummy32 = 0x34FF12AB; /* decimal 52 */
	uint32_t dummy16 = 0x12AB; /* decimal 52 */
	printf("%x to %x\n", dummy32, host2network32(dummy32));
	printf("%x to %x\n", host2network32(dummy32), network2host32(host2network32(dummy32)));
	printf("%x to %x\n", dummy16, host2network16(dummy16));
	printf("%x to %x\n", host2network16(dummy16), network2host16(host2network16(dummy16)));
	//insert_in_main_unittestsuit(argc, argv);
	return 0;
}