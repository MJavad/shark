#include "Misc/stdafx.h"
#include "../../../resource.h"
#include "ISizable.h"
#include "Base/WndProc.h"
#include "Base/D3DManager.h"
#include "Base/FileManager.h"

namespace UI {
namespace Components {
	boost::weak_ptr<ISizable> ISizable::s_activeSizer;
	Utils::Vector2 ISizable::s_sizeVector;

	ISizable::ISizable() : m_isGripVisible(true), m_isHovered(false), m_isSizeLimited(false) {
		m_resizeTexture = sD3DMgr.GetTextureFromResource(IDB_RESIZER_PNG);
		m_resizeTextureHover = sD3DMgr.GetTextureFromResource(IDB_RESIZER_HOVER_PNG);
	}

	void ISizable::OnRender(uint32 timePassed) {
		if (!m_isGripVisible)
			return;

		RECT sizerRect = GetSizerRect();
		Utils::Vector3 position(float(sizerRect.left), float(sizerRect.top), 0.0f);

		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr) {
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
			pSprite->Draw(m_isHovered ? m_resizeTextureHover : m_resizeTexture, nullptr,
				nullptr, &position, CalculateAbsoluteColor(0xFFFFFFFF));
			pSprite->End();
		}
	}

	void ISizable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsSizing() && !_notifyResizeEndEvent(nullptr))
				ResetActiveSizer();

			return;
		}

		Utils::Vector2 position(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			if (!sWndProc.LastMessageHandled &&
				!IsSizing() && PtInSizerRect(position)) {
				const auto activeSizer = GetActiveSizer();
				if (activeSizer != nullptr)
					activeSizer->_notifyResizeEndEvent(&position);

				Utils::Vector2 vSize = GetScreenPosition();
				vSize.x += GetWidth();
				vSize.y += GetHeight();

				if (!_notifyResizeBeginEvent(&position))
					StartSizing(vSize - position);

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_LBUTTONUP:
			if (IsSizing()) {
				if (!_notifyResizeEndEvent(&position)) {
					ResetActiveSizer();
					m_isHovered = PtInSizerRect(position);
				}

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (!sWndProc.LastMessageHandled && IsSizing() &&
				GetGlobalInterface()->ClipStack.PtInClipArea(position)) {
				Utils::Vector2 vSize = position - GetScreenPosition() + s_sizeVector;
				Utils::Vector2 vMinSize = GetMinSize();

				if (GetSizeLimited()) {
					Utils::Vector2 vMaxSize = GetMaxSize();
					if (vSize.x > vMaxSize.x)
						vSize.x = vMaxSize.x;

					if (vSize.y > vMaxSize.y)
						vSize.y = vMaxSize.y;
				}

				if (vSize.x < vMinSize.x)
					vSize.x = vMinSize.x;

				if (vSize.y < vMinSize.y)
					vSize.y = vMinSize.y;

				if (vSize.x < 0.0f)
					vSize.x = 0.0f;

				if (vSize.y < 0.0f)
					vSize.y = 0.0f;

				if (!_notifyResizeEvent(vSize.x, vSize.y)) {
					SetWidth(vSize.x);
					SetHeight(vSize.y);
				}
			}

			m_isHovered = IsSizing() || (!sWndProc.LastMessageHandled &&
										 PtInSizerRect(position) &&
										 GetActiveSizer() == nullptr);

			sWndProc.LastMessageHandled |= m_isHovered;
			break;
		};
	}

	void ISizable::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<ISizable, IRectComponent,
							boost::shared_ptr<IComponent>>("ISizable")
				.def("PtInSizerRect", &ISizable::PtInSizerRect)
				.def_readonly("resizeBeginEvent", &ISizable::OnResizeBeginLuaWrap)
				.def_readonly("resizeEndEvent", &ISizable::OnResizeEndLuaWrap)
				.def_readonly("resizeEvent", &ISizable::OnResizeLuaWrap)
				.property("isSizing", &ISizable::IsSizing)
				.property("gripVisible", &ISizable::GetGripVisibility, &ISizable::SetGripVisibility)
				.property("minSize", &ISizable::GetMinSize, &ISizable::SetMinSize)
				.property("maxSize", &ISizable::GetMaxSize, &ISizable::SetMaxSize)
				.property("maxSizeLimited", &ISizable::GetSizeLimited, &ISizable::SetSizeLimited)
				.property("sizerRect", &ISizable::GetSizerRect)
		];
	}
}
}
