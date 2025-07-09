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

		void printErr() {
			cout << err << '\n';
		}
	};
}

#include <CEvent.h>
#include <CEventQ.h>
#include <CPacket.h>
#include <CEntity.h>
#include <CRouter.h>

/* Router Algorithm Extensions for WDS */
extern "C" {
	void* WExtractHeadFrmDList(struct WDLList* l);
}