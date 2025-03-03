#include <des_sim_defs.h>

extern "C" {
#include <router_defs.h>
int insert_in_main_router_unittestsuit(int argc, char* argv[]);
};

//using namespace WDS;


int main(int argc, char* argv[])
{
	cout << "*** DES/Network Simulator App ***" << endl;

	try {
		
		insert_in_main_router_unittestsuit(argc, argv);
		
	}
	catch (...) {
		cerr << "Uncaught error! Simulation shutting down\n";
	}
	return 0;
}