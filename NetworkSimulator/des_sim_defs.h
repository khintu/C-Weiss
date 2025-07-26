#pragma once

extern "C" {
#include <router_defs.h>
};

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

/* DES Network Simulation Classes */

namespace WDS {
	class Exception {
		string err;
	public:
		Exception() {}
		Exception(const char* e) : err(e) {}
		Exception(string& str) : err(str) {}
		Exception(string str) : err(str) {}
		void printErr() {
			cerr << err << '\n';
		}
	};
}

#include <CEvent.h>
#include <CEventQ.h>
#include <CEntity.h>
#include <CPacketQ.h>
#include <CRouter.h>
#include <CRtrContainr.h>
#include <CPacket.h>
#include <CPktFwd.h>
#include <CGlbData.h>

extern WDS::CGlbData* gDataRepo;

#define RTR_HOME_IP_ADDR_MASK 0x10100000

/* Router Algorithm Extensions for WDS */
extern "C" {
	void* WExtractHeadFrmDList(struct WDLList* l);
}