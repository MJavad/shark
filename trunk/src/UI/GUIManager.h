#pragma once
#include "Components/IComponent.h"
#include "Components/Frame.h"

namespace UI {
	class GUIManager : public Utils::Singleton<GUIManager>
	{
		SINGLETON_OBJ(GUIManager);
		GUIManager() {}

	public:
		void Initialize();
		std::shared_ptr<Components::Frame> CreateBasicFrame(std::wstring swTitle,
			float fWidth, float fHeight, const Utils::Color &color) const;

		std::shared_ptr<ComponentInterface> GetInterface() const { return m_interface; }

	protected:
		std::shared_ptr<ComponentInterface> m_interface;
	};
}

#define sGUIMgr ::Utils::Singleton<::UI::GUIManager>::Instance()
