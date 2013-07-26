#pragma once
#include "Components/IComponent.h"
#include "Components/Frame.h"

namespace UI {
	class GUIManager {
		SINGLETON_CLASS(GUIManager) {}

	public:
		void Initialize();

		boost::shared_ptr<Components::Frame> CreateBasicFrame(
			std::wstring windowTitle, float width, float height, const D3DXCOLOR &color) const;

		boost::shared_ptr<Components::Frame> CreateBasicFrame_UTF8(
			const std::string &windowTitle, float width, float height, const D3DXCOLOR &color) const;

		boost::shared_ptr<ComponentInterface> GetInterface() const { return m_interface; }
		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		boost::shared_ptr<ComponentInterface> m_interface;
	};
}

#define sGUIMgr ::UI::GUIManager::Instance()
