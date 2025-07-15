#pragma once

namespace WDS {

	class CGlbData
	{
		static CGlbData* _instance;
		static CGlbData* getInstance();
		
		// Global data structures (members)
		struct WLList* inetList;
		CRtrContainr* routerCntr;
		
		// Private constructor and destructor to prevent instantiation
		CGlbData();
		~CGlbData();


	public:
		static void InitializeGlbData();
		static void DeleteGlbData();
	};
}

