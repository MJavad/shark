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
#include "UI/D3DTexture.h"

namespace UI {
namespace Components {
	class Rectangle : public virtual IRectComponent
	{
	public:
		Rectangle();
		~Rectangle();

		static boost::shared_ptr<Rectangle> Create(float width = 100.0f, float height = 100.0f);

		virtual void OnRender(uint32 timePassed);

		virtual float4 GetHorizontalRoundings() const {
			return m_horizRoundings;
		}

		virtual float4 GetVerticalRoundings() const {
			return m_vertRoundings;
		}

		virtual void SetHorizontalRoundings(const float4 &roundings) {
			m_horizRoundings = roundings;
			_flushTextureCache();
		}

		virtual void SetVerticalRoundings(const float4 &roundings) {
			m_vertRoundings = roundings;
			_flushTextureCache();
		}

		virtual void SetHorizontalRounding(float horizontalRounding) {
			m_horizRoundings._1 = horizontalRounding;
			m_horizRoundings._2 = horizontalRounding;
			m_horizRoundings._3 = horizontalRounding;
			m_horizRoundings._4 = horizontalRounding;
			_flushTextureCache();
		}

		virtual void SetVerticalRounding(float verticalRounding) {
			m_vertRoundings._1 = verticalRounding;
			m_vertRoundings._2 = verticalRounding;
			m_vertRoundings._3 = verticalRounding;
			m_vertRoundings._4 = verticalRounding;
			_flushTextureCache();
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			m_colors.fill(color);
			_flushTextureCache();
		}

		virtual void SetGradientColors(std::array<D3DXCOLOR, 4> gradient) {
			m_colors = std::move(gradient);
			_flushTextureCache();
		}

		virtual std::array<D3DXCOLOR, 4> GetGradientColors() const {
			return m_colors;
		}

		virtual void SetDropShadow(bool dropShadow) {
			m_dropShadow = dropShadow;
			if (!dropShadow)
				_flushTextureCache();
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

		virtual void SetDimensions(std::array<Utils::Vector2, 4> dimensions);

		virtual std::array<Utils::Vector2, 4> GetDimensions() const {
			return m_dimensions;
		}

		virtual void SetWidth(float width) {
			if (GetWidth() != width) {
				IRectComponent::SetWidth(width);
				_flushTextureCache();

				m_dimensions[0].x = 0; m_dimensions[1].x = width;
				m_dimensions[3].x = 0; m_dimensions[2].x = width;
			}
		}

		virtual void SetHeight(float height) {
			if (GetHeight() != height) {
				IRectComponent::SetHeight(height);
				_flushTextureCache();

				m_dimensions[0].y = 0; m_dimensions[3].y = height;
				m_dimensions[1].y = 0; m_dimensions[2].y = height;
			}
		}

		void CreateShadowTexture();
		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	private:
		bool m_dropShadow;
		Utils::Vector2 m_shadowDirection;
		boost::shared_ptr<D3DTexture> m_shadowTexture;

		float4 m_horizRoundings;
		float4 m_vertRoundings;
		std::array<D3DXCOLOR, 4> m_colors;
		std::array<Utils::Vector2, 4> m_dimensions;

		Utils::SEventDelegate<void ()> m_lostDevice;
		Utils::SEventDelegate<void ()> m_changeDevice;

		void _flushTextureCache() {
			m_shadowTexture.reset();
		}

		// wrapper for luabind ctor
		static boost::shared_ptr<Rectangle> _createLua() {
			const auto pRectangle = Create();
			pRectangle->_registerAsScriptElement();
			return pRectangle;
		}
	};
}
}
