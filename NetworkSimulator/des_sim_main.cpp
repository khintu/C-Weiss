#include <des_sim_defs.h>

extern "C" {
int insert_in_main_router_unittestsuit(int argc, char* argv[]);
int insert_in_main_algolib_unittestsuit(int argc, char* argv[]);
};

//using namespace WDS;
void testRudimentaryEventQueue();

int main(int argc, char* argv[])
{
	cout << "*** DES/Network Simulator App ***" << endl;
	//insert_in_main_algolib_unittestsuit(int argc, char* argv[]);
	//insert_in_main_router_unittestsuit(argc, argv);
	
	try {
		WDS::CGlbData::InitializeGlbData();
		testRudimentaryEventQueue();
	}
	catch (...) {
		cerr << "Uncaught error! Simulation shutting down\n";
	}
	WDS::CGlbData::DeleteGlbData();
	return 0;
}

void testRudimentaryEventQueue()
{
	try {
		WDS::CEventQ q;
		WDS::CEvent* e;
		int32_t i;

		for (i = 10; i > 0; i--) {
			e = new WDS::CPacket(i);
			q.enQueue(*e);
			delete e;
		}
		q.sort();
		for (e = &(q.deQueue()); e->getId() != 100; e = &(q.deQueue())) {
			//cout << "Event Id: " << e->getId() << '\n';
			e->execute();
			delete e;
		}
	}
	catch (WDS::Exception e) {
		e.printErr();
	}
	return;
}