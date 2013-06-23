#pragma once
#include "IRectComponent.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	class Label : public IRectComponent
	{
	public:
		Label();
		~Label();

		static std::shared_ptr<Label> Create(std::wstring swText = L"",
			uint32 dwFormatFlags = DT_NOCLIP | DT_SINGLELINE,
			float fWidth = 100.0f, float fHeight = 20.0f);
		
		virtual void OnRender(uint32 uTimePassed);

		void CreateCachedFontBatch();
		void RenderCachedFontBatch(const std::shared_ptr<const ID3DSprite> &pSprite) const;

		virtual std::wstring GetText() const {
			return m_text;
		}

		virtual void SetText(std::wstring swText) {
			m_text = std::move(swText);
			FlushFontCache();
		}

		virtual D3DXCOLOR GetColor() const {
			return m_color;
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			m_color = color;
		}

		virtual void SetWidth(float fWidth) {
			IRectComponent::SetWidth(fWidth);
			FlushFontCache();
		}

		virtual void SetHeight(float fHeight) {
			IRectComponent::SetHeight(fHeight);
			FlushFontCache();
		}

		virtual uint32 GetFormatFlags() const {
			return m_formatFlags;
		}

		virtual void SetFormatFlags(uint32 dwFormatFlags) {
			m_formatFlags = dwFormatFlags;
			FlushFontCache();
		}

		virtual std::shared_ptr<UI::D3DFont> GetFont() const {
			return m_font;
		}

		virtual void SetFont(std::shared_ptr<UI::D3DFont> pFont) {
			m_font = std::move(pFont);
			FlushFontCache();
		}

		virtual void SetDropShadow(bool bDropShadow) {
			m_dropShadow = bDropShadow;
		}

		virtual bool GetDropShadow() const {
			return m_dropShadow;
		}

		virtual void SetShadowDirection(const Utils::Vector2 &vShadowDirection) {
			m_shadowDirection = vShadowDirection;
		}

		virtual Utils::Vector2 GetShadowDirection() const {
			return m_shadowDirection;
		}

		virtual void SetUseCache(bool bUseCache) {
			m_shouldCache = bUseCache;
			if (!bUseCache)
				FlushFontCache();
		}

		virtual bool GetUseCache() const {
			return m_shouldCache;
		}

		bool IsCached() const {
			return m_fontCache != nullptr;
		}

		void FlushFontCache() {
			m_fontCache.reset();
		}

		uint32 XToCP(int32 x) const;
		int32 CPToX(uint32 cp) const;
		
	private:
		std::wstring m_text;
		D3DXCOLOR m_color;
		uint32 m_formatFlags;
		bool m_shouldCache, m_dropShadow;
		Utils::Vector2 m_shadowDirection;
		Utils::Vector2 m_textIndent;
		std::shared_ptr<D3DFont> m_font;
		std::shared_ptr<D3DTexture> m_fontCache;

		Utils::SEventDelegate<void ()> m_lostDevice;
		Utils::SEventDelegate<void ()> m_changeDevice;
	};
}
}
