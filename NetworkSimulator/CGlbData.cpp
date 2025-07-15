#include <des_sim_defs.h>

using namespace WDS;

CGlbData* gDataRepo;

CGlbData* CGlbData::_instance = NULL;

CGlbData* CGlbData::getInstance()
{
	if (_instance == NULL) {
			_instance = new CGlbData();
	}
	return _instance;
}

void CGlbData::InitializeGlbData()
{
	if (gDataRepo == NULL) {
		gDataRepo = getInstance();
	}
	return;
}

void CGlbData::DeleteGlbData()
{
	if (gDataRepo != NULL) {
		delete gDataRepo;
		gDataRepo = NULL;
	}
	return;
}

CGlbData::CGlbData()
{
	// Initialize global data structures here
	inetList = initializeInternetMap();
	return;
}

CGlbData::~CGlbData()
{
	// Clean up global data structures here
	resetEverythingInIntrnt(inetList);
	WDeleteList(inetList);
	return;
}