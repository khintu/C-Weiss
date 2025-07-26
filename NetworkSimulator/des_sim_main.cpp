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
		uint32_t j, coolDn, count = gDataRepo->getRouterContainer()->getCount();

		q = gDataRepo->getEventQueue();
		for (i = 1, j = 1; i <= 10; i++, ++j) {
			if (j >= count)
				j = 1;
			e = new WDS::CPacket((uint32_t)i, RTR_HOME_IP_ADDR_MASK | j, RTR_HOME_IP_ADDR_MASK | count);
			q->enQueue(*e);
			delete e;
		}
		for (e = &(q->deQueue()), coolDn = 0; e->getTimestamp() != 100; e = &(q->deQueue())) {
			e->execute();
			delete e;
			q->sort();
			if (coolDn++ >= 5) {
				coolDn = 0;
				gDataRepo->getRouterContainer()->processCoolDown();
			}
		}
	}
	catch (WDS::Exception& e) {
		e.printErr();
	}
	return;
}