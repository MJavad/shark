#pragma once
#include "Components/IComponent.h"
#include "Components/Frame.h"

namespace UI {
	class GUIManager {
		SINGLETON_CLASS(GUIManager) {}

	public:
		void Initialize();
		boost::shared_ptr<Components::Frame> CreateBasicFrame(std::wstring swTitle,
			float width, float height, const D3DXCOLOR &color) const;

		boost::shared_ptr<ComponentInterface> GetInterface() const { return m_interface; }

	protected:
		boost::shared_ptr<ComponentInterface> m_interface;
	};
}

#define sGUIMgr ::UI::GUIManager::Instance()
