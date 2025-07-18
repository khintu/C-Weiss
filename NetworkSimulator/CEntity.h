#pragma once

namespace WDS {
	class CEntity {
	public:
		CEntity() {}
		virtual void handleEvent(CEvent* e) = 0;
		virtual ~CEntity() {}
	};
}