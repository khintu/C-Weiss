#pragma once

namespace WDS {

	class CGlbData
	{
		// Singleton instance
		static CGlbData* _instance;
		static CGlbData* getInstance();
		
		// Global data structures (members)
		struct WLList* inetList;
		CRtrContainr* routerCntr;
		CEventQ* eventQ;
		
		// Private constructor and destructor to prevent instantiation
		CGlbData();
		~CGlbData();


	public:
		static void InitializeGlbData();
		static void DeleteGlbData();

		CRtrContainr* getRouterContainer() const {
			return routerCntr;
		}
		struct WLList* getInetList() const {
			return inetList;
		}
		CEventQ* getEventQueue() const {
			return eventQ;
		}
	};
}

