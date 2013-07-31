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
#include "Rectangle.h"
#include "Label.h"
#include "IHoverable.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	class ListBoxEntry : public Rectangle, public IHoverable
	{
	public:
		ListBoxEntry() : m_isSelected(false) {}
		
		static boost::shared_ptr<ListBoxEntry> CreateDefault() {
			return Create();
		}

		static boost::shared_ptr<ListBoxEntry> Create(std::wstring textString = L"Default Entry");

		virtual void OnRender(uint32 timePassed);
		void RenderCachedFontBatch(const boost::shared_ptr<const ID3DSprite> &pSprite) const;
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);
		
		virtual void SetColor(const D3DXCOLOR &color) {
			Rectangle::SetColor(color);
			m_isSelected ? m_selectedColors.fill(color):
						   m_deselectedColors.fill(color);
		}
		
		virtual void SetGradientColors(const std::array<D3DXCOLOR, 4> &gradient) {
			Rectangle::SetGradientColors(gradient);
			m_isSelected ? m_selectedColors = gradient :
						   m_deselectedColors = gradient;
		}

		virtual void SetSelectedColor(const D3DXCOLOR &color) {
			m_selectedColors.fill(color);
			if (m_isSelected)
				Rectangle::SetColor(color);
		}
		
		virtual void SetSelectedGradientColors(const std::array<D3DXCOLOR, 4> &gradient) {
			m_selectedColors = gradient;
			if (m_isSelected)
				Rectangle::SetGradientColors(gradient);
		}

		virtual std::array<D3DXCOLOR, 4> GetSelectedGradientColors() const {
			return m_selectedColors;
		}

		virtual void SetDeselectedColor(const D3DXCOLOR &color) {
			m_deselectedColors.fill(color);
			if (!m_isSelected)
				Rectangle::SetColor(color);
		}
		
		virtual void SetDeselectedGradientColors(const std::array<D3DXCOLOR, 4> &gradient) {
			m_deselectedColors = gradient;
			if (!m_isSelected)
				Rectangle::SetGradientColors(gradient);
		}

		virtual std::array<D3DXCOLOR, 4> GetDeselectedGradientColors() const {
			return m_deselectedColors;
		}

		virtual bool GetSelected() const {
			return m_isSelected;
		}

		virtual void SetSelected(bool selected) {
			m_isSelected = selected;
			Rectangle::SetGradientColors(m_isSelected ? m_selectedColors : m_deselectedColors);
		}

		virtual boost::shared_ptr<Label> GetCaption() const {
			return m_caption;
		}

		virtual void SetCaption(boost::shared_ptr<Label> pCaption) {
			m_caption = std::move(pCaption);
			if (m_caption != nullptr)
				m_caption->SetUIParent(get_this<IComponent>());
		}

		virtual boost::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(boost::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr) {
				m_border->SetPosition(Utils::Vector2(-1.0f, -1.0f));
				m_border->SetUIParent(get_this<IComponent>());
			}
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		bool m_isSelected;
		boost::shared_ptr<Label> m_caption;
		boost::shared_ptr<Rectangle> m_border;
		std::array<D3DXCOLOR, 4> m_deselectedColors, m_selectedColors;
	};
}
}