#include "Misc/stdafx.h"
#include "ISizable.h"
#include "Base/WndProc.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	std::weak_ptr<ISizable> ISizable::s_activeSizer;
	Utils::Vector2 ISizable::s_sizeVector;

	std::shared_ptr<D3DTexture> ISizable::s_resizeTexture;
	std::shared_ptr<D3DTexture> ISizable::s_resizeTextureHover;

	ISizable::ISizable() : m_isGripVisible(true), m_isHovered(false), m_isSizeLimited(false) {
		if (s_resizeTexture == nullptr)
			s_resizeTexture = sD3DMgr.GetTextureFromFile(L"E:\\shark\\resizer.png");

		if (s_resizeTextureHover == nullptr)
			s_resizeTextureHover = sD3DMgr.GetTextureFromFile(L"E:\\shark\\resizer_hover.png");
	}

	void ISizable::OnRender(uint32 uTimePassed) {
		if (!m_isGripVisible)
			return;

		RECT sizerRect = GetSizerRect();
		Utils::Vector3 vPosition(float(sizerRect.left), float(sizerRect.top));

		const auto pSprite = sD3DMgr.GetSprite();
		if (pSprite != nullptr) {
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
			pSprite->Draw(m_isHovered ? s_resizeTextureHover : s_resizeTexture,
				nullptr, nullptr, &vPosition, GetModifiedColor(0xFFFFFFFF));
			pSprite->End();
		}
	}

	void ISizable::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (!IsVisible()) {
			if (IsSizing() && !OnResizeEndRequest(nullptr))
				ResetActiveSizer();

			return;
		}

		Utils::Vector2 vPosition(lParam);
		IComponent::OnMessageReceived(uMsg, wParam, lParam);

		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			if (!sWndProc.LastMessageHandled) {
				Utils::Vector2 vSize = GetScreenPosition();
				vSize.x += GetWidth();
				vSize.y += GetHeight();

				if (!IsSizing() && PtInSizerRect(vPosition) &&
					!OnResizeStartRequest(&vPosition))
					StartSizing(vSize - vPosition);

				sWndProc.LastMessageHandled |= IsSizing();
			}
			break;

		case WM_LBUTTONUP:
			if (IsSizing()) {
				if (!OnResizeEndRequest(&vPosition))
					ResetActiveSizer();

				sWndProc.LastMessageHandled = true;
			}
			break;

		case WM_MOUSEMOVE:
			if (!sWndProc.LastMessageHandled &&
				IsSizing() && GetInterface()->ClipStack.PtInClipArea(vPosition)) {
				Utils::Vector2 vSize = vPosition - GetScreenPosition() + s_sizeVector;
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

				if (!OnResizeRequest(vSize.x, vSize.y)) {
					SetWidth(vSize.x);
					SetHeight(vSize.y);
				}
			}

			m_isHovered = IsSizing() || (!sWndProc.LastMessageHandled &&
										 PtInSizerRect(vPosition));
			sWndProc.LastMessageHandled |= m_isHovered;
			break;
		};
	}
}
}
