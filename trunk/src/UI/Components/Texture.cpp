#include "Misc/stdafx.h"
#include "Texture.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	std::shared_ptr<Texture> Texture::Create(std::shared_ptr<D3DTexture> pTexture) {
		const auto pControl = std::make_shared<Texture>();
		pControl->SetTexture(std::move(pTexture));
		return pControl;
	}

	void Texture::OnRender(uint32 uTimePassed) {
		const auto pTexture = GetTexture();
		if (pTexture == nullptr)
			return;

		Utils::Vector3 vScreen = GetScreenPosition();
		RECT screenRect = GetFullRect();
		const auto pSprite = sD3DMgr->GetSprite();

		if (pSprite != nullptr && IsRectEmpty(&screenRect) != FALSE) {
			D3DXMATRIX matTransform = GetTransform();
			pSprite->SetTransform(&matTransform);
			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
			pSprite->Draw(pTexture, nullptr, nullptr, &vScreen, GetModifiedColor(GetColor()));
			pSprite->End();
			pSprite->SetTransform(nullptr);
		}
		else {
			float fWidth = GetWidth();
			float fHeight = GetHeight();
			const auto pRenderTarget = sD3DMgr->GetRenderTarget();
			const auto dimensions = pRenderTarget->MakeDimension(fWidth, fHeight);
			pRenderTarget->DrawSprite(vScreen, pTexture, dimensions, GetModifiedColor(GetColor()));
		}
	}
}
}
