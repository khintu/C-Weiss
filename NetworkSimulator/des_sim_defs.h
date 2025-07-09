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
#include <CEvent.h>
#include <CEventQ.h>
#include <CPacket.h>

namespace WDS {
	class Exception {
		string err;
	public:
		Exception() {}
		Exception(const char* e): err(e) {}
		
		void printErr() {
			cout << err << '\n';
		}
	};

	class CEntity {
	public:
		CEntity() {}
		virtual void handleEvent(CEvent* e) = 0;
		virtual ~CEntity() {}
	};
}

#include <CRouter.h>

/* Router Algorithm Extensions for WDS */
extern "C" {
	void* WExtractHeadFrmDList(struct WDLList* l);
}