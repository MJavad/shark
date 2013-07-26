#include "Misc/stdafx.h"
#include "Texture.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	boost::shared_ptr<Texture> Texture::Create(boost::shared_ptr<D3DTexture> pTexture) {
		const auto pControl = boost::make_shared<Texture>();
		pControl->SetTexture(std::move(pTexture));
		return pControl;
	}

	void Texture::OnRender(uint32 timePassed) {
		const auto pTexture = GetTexture();
		if (pTexture == nullptr)
			return;

		Utils::Vector3 vScreen = GetScreenPosition();
		RECT screenRect = GetFullRect();
		const auto pSprite = sD3DMgr.GetSprite();

		if (pSprite != nullptr && IsRectEmpty(&screenRect) != FALSE) {
			D3DXMATRIX matTransform = GetTransform();
			pSprite->SetTransform(&matTransform);

			pSprite->Begin(D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
			pSprite->Draw(pTexture, nullptr, nullptr, &vScreen, CalculateAbsoluteColor(GetColor()));
			pSprite->End();

			pSprite->SetTransform(nullptr);
		}
		else {
			std::array<D3DXCOLOR, 4> gradient;
			gradient.fill(CalculateAbsoluteColor(GetColor()));
			const auto dimensions = IRenderTarget::MakeDimension(GetWidth(), GetHeight());
			sD3DMgr.GetRenderTarget()->DrawSprite(vScreen, pTexture, dimensions, gradient);
		}
	}

	void Texture::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<Texture, IRectComponent,
							boost::shared_ptr<IComponent>>("TextureControl")
				.scope [ luabind::def("Create", &Texture::CreateDefault) ]
				.property("texture", &Texture::GetTexture, &Texture::SetTexture)
				.property("transform", &Texture::GetTransform, &Texture::SetTransform)
				.property("color", &Texture::GetColor, &Texture::SetColor)
		];
	}
}
}
