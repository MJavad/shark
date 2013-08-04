/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "IRectComponent.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	class Label : public virtual IRectComponent
	{
	public:
		Label();
		~Label();

		static boost::shared_ptr<Label> Create(
			std::wstring textString = L"Default Label",
			uint32 formatFlags = DT_LEFT | DT_TOP,
			float width = 100.0f, float height = 20.0f);
		
		virtual void OnRender(uint32 timePassed);

		void CreateCachedFontBatch();
		void RenderCachedFontBatch(const boost::shared_ptr<const ID3DSprite> &pSprite) const;

		virtual void SetWidth(float width);
		virtual void SetHeight(float height);

		virtual const std::wstring& GetText() const {
			return m_text;
		}

		std::string GetText_UTF8() const {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.to_bytes(GetText());
		}

		virtual void SetText(std::wstring textString) {
			if (m_text != textString) {
				m_text = std::move(textString);
				_flushFontCache();
			}
		}

		void SetText_UTF8(const std::string &textString) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			SetText(conv.from_bytes(textString));
		}

		virtual D3DXCOLOR GetColor() const {
			return m_color;
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			m_color = color;
		}

		virtual uint32 GetFormatFlags() const {
			return m_formatFlags;
		}

		virtual void SetFormatFlags(uint32 formatFlags) {
			if (m_formatFlags != formatFlags) {
				m_formatFlags = formatFlags;
				_flushFontCache();
			}
		}

		virtual boost::shared_ptr<UI::D3DFont> GetFont() const {
			return m_font;
		}

		virtual void SetFont(boost::shared_ptr<UI::D3DFont> pFont) {
			if (m_font != pFont) {
				m_font = std::move(pFont);
				_flushFontCache();
			}
		}

		virtual void SetDropShadow(bool dropShadow) {
			m_dropShadow = dropShadow;
		}

		virtual bool GetDropShadow() const {
			return m_dropShadow;
		}

		virtual void SetShadowDirection(const Utils::Vector2 &shadowDirection) {
			m_shadowDirection = shadowDirection;
		}

		virtual Utils::Vector2 GetShadowDirection() const {
			return m_shadowDirection;
		}

		virtual void SetUseCache(bool useCache) {
			m_shouldCache = useCache;
			if (!useCache)
				_flushFontCache();
		}

		virtual bool GetUseCache() const {
			return m_shouldCache;
		}

		bool IsCached() const {
			return m_fontCache != nullptr;
		}

		uint32 XToCP(int32 x) const;
		int32 CPToX(uint32 cp) const;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);
		
	private:
		std::wstring m_text;
		D3DXCOLOR m_color;
		uint32 m_formatFlags;
		bool m_shouldCache, m_dropShadow;
		Utils::Vector2 m_shadowDirection;
		Utils::Vector2 m_textOffset;
		boost::shared_ptr<D3DFont> m_font;
		boost::shared_ptr<D3DTexture> m_fontCache;

		Utils::SEventDelegate<void ()> m_lostDevice;
		Utils::SEventDelegate<void ()> m_changeDevice;

		void _recalcTextMetrics();

		void _flushFontCache() {
			m_fontCache.reset();
		}

		// wrapper for luabind ctor
		static boost::shared_ptr<Label> _createLua() {
			const auto pLabel = Create();
			pLabel->RegisterAsScriptElement();
			return pLabel;
		}
	};
}
}
