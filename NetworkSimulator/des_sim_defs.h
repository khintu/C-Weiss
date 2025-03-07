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

/* DES Network Routers Classes */
#include <CEvent.h>

using namespace std;

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
}

/* Router Algorithm Extensions for WDS */
extern "C" {
	void* WExtractHeadFrmDList(struct WDLList* l);
}