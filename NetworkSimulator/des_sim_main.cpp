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
		WDS::CEventQ* q;
		WDS::CEvent* e;
		int32_t i;
		uint32_t j, count = gDataRepo->getRouterContainer()->getCount();

		q = gDataRepo->getEventQueue();
		for (i = 50, j = 1; i > 0; i--, ++j) {
			if (j >= count)
				j = 1;
			e = new WDS::CPacket((uint32_t)i, RTR_HOME_IP_ADDR_MASK | j, RTR_HOME_IP_ADDR_MASK | count);
			q->enQueue(*e);
			delete e;
		}
		q->sort();
		for (e = &(q->deQueue()); e->getTimestamp() != 100; e = &(q->deQueue())) {
			e->execute();
			delete e;
		}
	}
	catch (WDS::Exception e) {
		e.printErr();
	}
	return;
}