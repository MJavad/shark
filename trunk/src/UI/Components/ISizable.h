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
#include "IComponent.h"
#include "IRectComponent.h"
#include "UI/D3DTexture.h"

namespace UI {
namespace Components {
	class ISizable abstract : public virtual IRectComponent
	{
	public:
		virtual ~ISizable() {
			if (IsSizing())
				s_activeSizer.reset();
		}

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool IsSizing() const {
			return GetActiveSizer().get() == this;
		}

		static void ResetActiveSizer() {
			s_activeSizer.reset();
		}

		static boost::shared_ptr<ISizable> GetActiveSizer() {
			return s_activeSizer.lock();
		}

		void StartSizing(const Utils::Vector2 &vSizeVector) {
			s_activeSizer = get_this<ISizable>();
			s_sizeVector = vSizeVector;
		}

		bool GetGripVisibility() const {
			return m_isGripVisible;
		}

		void SetGripVisibility(bool gripVisible) {
			m_isGripVisible = gripVisible;
		}

		bool GetSizeLimited() const {
			return m_isSizeLimited;
		}

		void SetSizeLimited(bool sizeLimited) {
			m_isSizeLimited = sizeLimited;
		}

		Utils::Vector2 GetMinSize() const {
			return m_minSize;
		}

		void SetMinSize(const Utils::Vector2 &minSize) {
			m_minSize = minSize;
		}

		Utils::Vector2 GetMaxSize() const {
			return m_maxSize;
		}

		void SetMaxSize(const Utils::Vector2 &maxSize) {
			m_maxSize = maxSize;
			SetSizeLimited(true);
		}

		RECT GetSizerRect() const {
			RECT sizerRect = GetFullRect();
			sizerRect.left = sizerRect.right - 16;
			sizerRect.top = sizerRect.bottom - 16;
			return sizerRect;
		}

		bool PtInSizerRect(const POINT &vPoint) const {
			RECT sizeRect = GetSizerRect();
			return PtInRect(&sizeRect, vPoint) != FALSE;
		}

		// lua wrapper
		Utils::Event<void ()> OnResizeBeginLuaWrap;
		Utils::Event<void ()> OnResizeEndLuaWrap;
		Utils::Event<void ()> OnResizeLuaWrap;

		// returning true = no size
		Utils::Event<bool (const boost::shared_ptr<ISizable>&, Utils::Vector2*)> OnResizeBeginEvent;
		Utils::Event<bool (const boost::shared_ptr<ISizable>&, Utils::Vector2*)> OnResizeEndEvent;
		Utils::Event<bool (const boost::shared_ptr<ISizable>&, float, float)> OnResizeEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		ISizable();

		virtual bool _notifyResizeBeginEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: ResizeStart triggered.", this);
			OnResizeBeginLuaWrap();
			return OnResizeBeginEvent(get_this<ISizable>(), pPosition);
		}

		virtual bool _notifyResizeEndEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: ResizeEnd triggered.", this);
			OnResizeEndLuaWrap();
			return OnResizeEndEvent(get_this<ISizable>(), pPosition);
		}

		virtual bool _notifyResizeEvent(float width, float height) {
			OnResizeLuaWrap();
			return OnResizeEvent(get_this<ISizable>(), width, height);
		}

	private:
		bool m_isGripVisible;
		bool m_isHovered;

		bool m_isSizeLimited;
		Utils::Vector2 m_minSize, m_maxSize;

		boost::shared_ptr<D3DTexture> m_resizeTexture;
		boost::shared_ptr<D3DTexture> m_resizeTextureHover;

		static boost::weak_ptr<ISizable> s_activeSizer;
		static Utils::Vector2 s_sizeVector;
	};
}
}
