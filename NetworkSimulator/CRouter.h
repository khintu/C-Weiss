#pragma once

namespace WDS {

	class CRouter : public CEntity {
	
	public:
		CRouter() {}
		virtual void handleEvent(CEvent* e) {}
		virtual ~CRouter() { CEntity::~CEntity(); }
		
	};
}
