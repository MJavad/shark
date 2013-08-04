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
#include "Label.h"
#include "Rectangle.h"
#include "IFocusable.h"
#include "IPushable.h"
#include "Misc/LuaEvent.h"

namespace UI {
namespace Components {
	class EditBox : public virtual Rectangle,
					public virtual IFocusable,
					public virtual IPushable
	{
	public:
		EditBox() : m_scrollPosition(0.0f), m_maxLength(80) {}

		static boost::shared_ptr<EditBox> Create(
			bool centerAlign = false,
			float width = 200.0f, float height = 25.0f,
			float horizontalRounding = 6.0f, float verticalRounding = 3.0f);

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
			return IFocusable::OnTabPressed(pComponent);
		}

		virtual void SetWidth(float width);
		virtual void SetHeight(float height);

		virtual boost::shared_ptr<Label> GetContent() const {
			return m_content;
		}

		virtual void SetContent(boost::shared_ptr<Label> pContent) {
			m_content = std::move(pContent);
			if (m_content != nullptr)
				m_content->SetUIParent(getThis<IComponent>());
		}

		virtual boost::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(boost::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr)
				m_border->SetUIParent(getThis<IComponent>());
		}

		std::wstring GetCurrentText() const;
		bool CopyToClipboard() const;
		uint32 PasteFromClipboard();

		// lua wrapper
		Utils::LuaEvent<void ()> OnContentChangedLuaWrap;
		Utils::Event<void (const boost::shared_ptr<EditBox>&)> OnContentChangedEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		virtual void _notifyPushEvent(Utils::Vector2 *pPosition);
		virtual void _notifyDblClickEvent(Utils::Vector2 *pPosition);
		virtual void _notifyReleaseEvent(Utils::Vector2 *pPosition);

		virtual bool _notifyFocusBeginEvent();
		virtual void _notifyFocusEndEvent();

		virtual void _notifyContentChangedEvent() {
			OnContentChangedLuaWrap();
			OnContentChangedEvent(getThis<EditBox>());
		}

	private:
		static bool s_renderCaret;
		static uint32 s_caretPosition;
		static uint32 s_caretTimer;

		static bool s_activeSelection;
		static bool s_swapSelection;
		static bool s_handleDblClick;
		static bool s_pressedOnFocus;
		static uint32 s_selectPosition1, s_selectPosition2;

		uint32 m_maxLength;
		float m_scrollPosition;
		boost::shared_ptr<Label> m_content;
		boost::shared_ptr<Rectangle> m_border;

		void _onChar(wchar_t c);
		void _onKeyDown(uint32 keyCode);
		void _onMouseMove(const Utils::Vector2 &position);

		static void _resetCaret() {
			s_renderCaret = true;
			s_caretTimer = timeGetTime();
		}

		static void _clearSelection() {
			s_swapSelection = false;
			s_selectPosition1 = s_caretPosition;
			s_selectPosition2 = s_caretPosition;
		}

		static uint32 _getSelectionCount() {
			return std::labs(s_selectPosition1 - s_selectPosition2);
		}

		bool _hasSelection() const {
			return IsFocused() && s_selectPosition1 != s_selectPosition2;
		}

		void _eraseSelection() {
			uint32 numChars = _getSelectionCount();
			uint32 writePosition = _getWritePosition();
			_clearSelection();
			_eraseText(writePosition, numChars);
			_placeCaret(writePosition);
		}

		void _selectRange(uint32 position, uint32 numChars) {
			const auto pContent = GetContent();
			if (pContent != nullptr) {
				uint32 length = pContent->GetText().length();
				s_selectPosition1 = min(position, length);
				s_selectPosition2 = min(position + numChars, length);
			}
		}

		void _selectAll() {
			s_swapSelection = true;
			_selectRange(0, UINT_MAX);
		}

		uint32 _getWritePosition() const {
			if (!_hasSelection())
				return s_caretPosition;

			return min(s_selectPosition1, s_selectPosition2);
		}

		bool _scrollTo(uint32 position);
		void _placeCaret(uint32 position, bool select = false);

		uint32 _insertText(std::wstring textString);
		uint32 _insertText(uint32 insertPosition, std::wstring textString);

		uint32 _eraseText(uint32 numChars);
		uint32 _eraseText(uint32 erasePosition, uint32 numChars);

		// wrapper for luabind ctor
		static boost::shared_ptr<EditBox> _createLua() {
			const auto pEditBox = Create();
			pEditBox->RegisterAsScriptElement();
			return pEditBox;
		}
	};
}
}
