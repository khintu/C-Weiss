#include <router_defs.h>

extern int insert_in_main_unittestsuit(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	uint8_t dummy = 0x34; /* decimal 52 */
	printf("%d\n", dummy);
	//insert_in_main_unittestsuit(argc, argv);
	return 0;
}