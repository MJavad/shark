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
#include "UI/ComponentInterface.h"

namespace UI {
	class ComponentInterface;

namespace Components {
	class IComponent abstract : public boost::enable_shared_from_this<IComponent>
	{
	public:
		virtual ~IComponent();

		virtual void OnRender(uint32 timePassed) = 0;
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {}

		std::list<boost::shared_ptr<IComponent>> GetUIHierarchy();
		std::list<boost::shared_ptr<const IComponent>> GetUIHierarchy() const;
		boost::shared_ptr<ComponentInterface> GetGlobalInterface() const;
		Utils::Vector2 GetScreenPosition() const;
		D3DXCOLOR CalculateAbsoluteColor(const D3DXCOLOR &color) const;
		std::array<D3DXCOLOR, 4> CalculateAbsoluteColor(const std::array<D3DXCOLOR, 4> &gradient) const;

		bool IsVisible() const;
		bool HasParent(const boost::shared_ptr<const IComponent> &pParent) const;

		void StopFade() { m_fadeActive = false; }
		void FadeTo(uint32 fadeTime, const D3DXCOLOR &fadeColor);
		void UndoFade();

		void StopMove() { m_moveActive = false; }
		void MoveTo(uint32 moveTime, const Utils::Vector2 &position);
		void UndoMove();

		virtual void Hide(uint32 time = 200);
		virtual void Show(uint32 time = 200);

		virtual bool GetVisibility() const {
			return m_isVisible;
		}

		virtual void SetVisibility(bool visible) {
			if (!visible && m_fadeActive) {
				m_fadeActive = false;
				SetColorMod(m_fadeTo);
			}

			m_isVisible = visible;
		}

		virtual Utils::Vector2 GetPosition() const {
			return m_position;
		}

		virtual void SetPosition(const Utils::Vector2 &position) {
			m_position = position;
		}

		virtual Utils::Vector2 GetChildOffset() const {
			return m_childOffset;
		}

		virtual void SetChildOffset(const Utils::Vector2 &childOffset) {
			m_childOffset = childOffset;
		}

		virtual D3DXCOLOR GetColorMod() const {
			return m_colorMod;
		}

		virtual void SetColorMod(const D3DXCOLOR &colorMod) {
			m_colorMod = colorMod;
		}

		virtual boost::shared_ptr<IComponent> GetUIParent() const {
			return m_parent.lock();
		}

		virtual void SetUIParent(boost::shared_ptr<IComponent> pParent) {
			m_parent = std::move(pParent);
		}

		virtual boost::shared_ptr<ComponentInterface> GetLocalInterface() const {
			return m_interface.lock();
		}

		virtual void SetLocalInterface(boost::shared_ptr<ComponentInterface> pInterface) {
			m_interface = std::move(pInterface);
		}

		virtual bool OnParentChanged(const boost::shared_ptr<IComponent> &pParent) {
			return true;
		}

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
			LOG_DEBUG("%08X: TabPressed triggered.", this);
			const auto pParent = GetUIParent();
			if (pParent != nullptr)
				return pParent->OnTabPressed(shared_from_this());

			return false;
		}

		virtual void OnParentSizeChanged(float width, float height) {}

		Utils::Vector2& GetPosition_Ref() {
			return m_position;
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IComponent();
		virtual void OnUpdate(uint32 timePassed);

		template <typename T>
		boost::shared_ptr<T> get_this() {
			return boost::dynamic_pointer_cast<T>(shared_from_this());
		}

		template <typename T>
		boost::shared_ptr<const T> get_this() const {
			return boost::dynamic_pointer_cast<const T>(shared_from_this());
		}

	private:
		bool m_isVisible;
		D3DXCOLOR m_colorMod;

		bool m_fadeActive, m_prevFade;
		uint32 m_fadeTime, m_fadeTimePassed;
		D3DXCOLOR m_fadeTo, m_fadeSrc;

		bool m_moveActive, m_prevMove;
		uint32 m_moveTime, m_moveTimePassed;
		Utils::Vector2 m_moveTo, m_moveSrc;

		Utils::Vector2 m_position, m_childOffset;

		boost::weak_ptr<IComponent> m_parent;
		boost::weak_ptr<ComponentInterface> m_interface;

		Utils::SEventDelegate<void (uint32)> m_updateDelegate;
	};
}
}
