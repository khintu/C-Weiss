#include <des_sim_defs.h>

extern "C" {
int insert_in_main_router_unittestsuit(int argc, char* argv[]);
};

//using namespace WDS;


int main(int argc, char* argv[])
{
	cout << "*** DES/Network Simulator App ***" << endl;
	//insert_in_main_router_unittestsuit(argc, argv);

	try {
		WDS::CEventQ q;
		WDS::CEvent a(10);
		WDS::CEvent b(5);
		q.enQueue(a);
		q.enQueue(b);
		for (WDS::CEvent& e = q.deQueue(); e.getId() != 0; e = q.deQueue())
			cout << "Event Id: " << e.getId() << '\n';
			;
	}
	catch (WDS::Exception e) {
		e.printErr();
	}
	catch (...) {
		cerr << "Uncaught error! Simulation shutting down\n";
	}
	return 0;
}