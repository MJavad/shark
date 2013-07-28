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

#include "Misc/stdafx.h"
#include "EditBox.h"
#include "Base/WndProc.h"
#include "Base/D3DManager.h"

namespace UI {
namespace Components {
	bool EditBox::s_renderCaret = false;
	uint32 EditBox::s_caretPosition = 3;
	uint32 EditBox::s_caretTimer = 0;
	bool EditBox::s_activeSelection = false;
	bool EditBox::s_swapSelection = false;
	bool EditBox::s_handleDblClick = false;
	bool EditBox::s_pressedOnFocus = false;
	uint32 EditBox::s_selectPosition1 = 0;
	uint32 EditBox::s_selectPosition2 = 0;

	boost::shared_ptr<EditBox> EditBox::Create(bool centerAlign,
											   float width,
											   float height,
											   float horizontalRounding,
											   float verticalRounding)
	{
		uint32 alignFlags = DT_VCENTER | DT_SINGLELINE;
		if (centerAlign)
			alignFlags |= DT_CENTER;

		// label is indented by 5 px.
		const auto pLabel = Label::Create(L"", alignFlags, max(width - 10.0f, 0.0f), height);
		pLabel->SetPosition(Utils::Vector2(5.0f, 0.0f));

		const auto pBorder = Rectangle::Create(width, height);
		pBorder->SetColor(0x90B0B0B0);
		pBorder->SetHorizontalRounding(horizontalRounding);
		pBorder->SetVerticalRounding(verticalRounding);
		pBorder->SetPosition(Utils::Vector2(-1.0f, -1.0f));

		const auto pEditBox = boost::make_shared<EditBox>();
		pEditBox->SetContent(std::move(pLabel));
		pEditBox->SetBorder(std::move(pBorder));
		pEditBox->SetWidth(width);
		pEditBox->SetHeight(height);
		pEditBox->SetHorizontalRounding(horizontalRounding);
		pEditBox->SetVerticalRounding(verticalRounding);

		std::array<D3DXCOLOR, 4> gradient;
		gradient[0] = 0xFF303030;
		gradient[1] = 0xFF303030;
		gradient[2] = 0xFF101010;
		gradient[3] = 0xFF101010;
		pEditBox->SetGradientColors(std::move(gradient));
		return pEditBox;
	}

	void EditBox::OnRender(uint32 timePassed) {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr && pBorder->GetVisibility())
			pBorder->OnRender(timePassed);

		Rectangle::OnRender(timePassed);

		const auto pContent = GetContent();
		if (pContent == nullptr)
			return; // no text & font to render...

		// apply clipping...
		RECT clipArea = GetFullRect();
		Utils::Vector2 textOffset = pContent->GetPosition();
		clipArea.left += static_cast<LONG>(textOffset.x);
		clipArea.top += static_cast<LONG>(textOffset.y);
		clipArea.right -= static_cast<LONG>(textOffset.x);
		clipArea.bottom -= static_cast<LONG>(textOffset.y);

		// check if clipping area is valid
		if (clipArea.left >= clipArea.right ||
			clipArea.top >= clipArea.bottom)
			return;

		// label and selection is indent clipped
		const auto pInterface = GetGlobalInterface();
		pInterface->ClipStack.Push(clipArea);
		pInterface->ClipStack.Apply();

		// text is scrolled left -> -m_scrollPosition
		SetChildOffset(Utils::Vector2(-m_scrollPosition, 0.0f));
		pContent->OnRender(timePassed);

		// caret is only drawn if the editbox has focus
		if (IsFocused()) {
			// get the font's height
			float textHeight = 15;
			float controlHeight = GetHeight();

			const auto pFont = pContent->GetFont();
			if (pFont != nullptr)
				textHeight = static_cast<float>(pFont->GetDescription().height);

			Utils::Vector2 vScreen = GetScreenPosition();
			const auto pRenderTarget = sD3DMgr.GetRenderTarget();

			if (_hasSelection()) {
				float transformed1 = static_cast<float>(pContent->CPToX(s_selectPosition1));
				float transformed2 = static_cast<float>(pContent->CPToX(s_selectPosition2));

				Utils::Vector2 vSelection;
				vSelection.x = min(transformed1, transformed2);
				vSelection.y = vScreen.y + (controlHeight / 2.0f - textHeight / 2.0f);
				float width = std::fabs(transformed1 - transformed2);

				D3DXCOLOR textColor = pContent->GetColor();
				textColor.a *= 0.35f;
				textColor.b *= 1.4f;

				std::array<D3DXCOLOR, 4> gradient;
				gradient.fill(pContent->CalculateAbsoluteColor(textColor));
				pRenderTarget->FillRectangle(vSelection, IRenderTarget::MakeDimension(width, textHeight), gradient);
			}

			// caret blink update...
			uint32 timeNow = timeGetTime();
			if (timeNow - s_caretTimer > 600) {
				s_caretTimer = timeNow;
				s_renderCaret = !s_renderCaret;
			}

			// draw caret...
			if (s_renderCaret) {
				// the screen coordinates of the caret
				Utils::Vector2 vCaret;
				vCaret.x = static_cast<float>(pContent->CPToX(s_caretPosition));
				vCaret.y = vScreen.y + (controlHeight / 2.0f - textHeight / 2.0f);

				// increase clipping area by 3 px, so the caret can render at the end
				clipArea.right += 4;
				pInterface->ClipStack.Pop();
				pInterface->ClipStack.Push(clipArea);
				pInterface->ClipStack.Apply();

				// caret and text have equal colors
				std::array<D3DXCOLOR, 4> gradient;
				gradient.fill(pContent->CalculateAbsoluteColor(pContent->GetColor()));
				pRenderTarget->FillRectangle(vCaret, IRenderTarget::MakeDimension(3.0f, textHeight), gradient);
			}
		}

		// restore child offset
		SetChildOffset(Utils::Vector2(0.0f, 0.0f));

		// restore clipping
		pInterface->ClipStack.Pop();
		pInterface->ClipStack.Apply();
	}

	void EditBox::OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (IsFocused()) {
			switch (uMsg)
			{
			case WM_LBUTTONDOWN:
				if (sWndProc.LastMessageHandled ||
					!PtInBoundingRect(Utils::Vector2(lParam)))
					Unfocus();
				break;

			case WM_CHAR:
				if (!sWndProc.LastMessageHandled)
					_onChar(wParam & 0xFFFF);
				break;

			case WM_KEYDOWN:
				if (!sWndProc.LastMessageHandled)
					_onKeyDown(wParam);
				break;

			case WM_MOUSEMOVE:
				if (!sWndProc.LastMessageHandled)
					_onMouseMove(Utils::Vector2(lParam));
				break;
			}
		}

		IPushable::OnMessageReceived(uMsg, wParam, lParam);
		IFocusable::OnMessageReceived(uMsg, wParam, lParam);
	}

	void EditBox::SetWidth(float width) {
		Rectangle::SetWidth(width);

		const auto pContent = GetContent();
		if (pContent != nullptr) {
			float indent = pContent->GetPosition().x;
			pContent->SetWidth(max(width - indent * 2.0f, 0.0f));
		}

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetWidth(width + 2);
	}

	void EditBox::SetHeight(float height) {
		Rectangle::SetHeight(height);

		const auto pContent = GetContent();
		if (pContent != nullptr)
			pContent->SetHeight(height);

		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->SetHeight(height + 2);
	}

	// If we have a selection it returns the selected text, otherwise it returns everything
	std::wstring EditBox::GetCurrentText() const {
		std::wstring result;
		const auto pContent = GetContent();
		if (pContent != nullptr) {
			result = pContent->GetText();
			uint32 selectionStart = min(s_selectPosition1, s_selectPosition2);
			uint32 selectionEnd = max(s_selectPosition1, s_selectPosition2);

			if (_hasSelection() && selectionEnd <= result.length())
				result = result.substr(selectionStart, selectionEnd - selectionStart);
		}

		return result;
	}

	bool EditBox::CopyToClipboard() const {
		return Utils::OsClipboardPutString(GetCurrentText());
	}

	uint32 EditBox::PasteFromClipboard() {
		return _insertText(Utils::OsClipboardGetString());
	}

	void EditBox::_onChar(wchar_t c) {
		switch (c) {
		case 0x01: // ctrl + a
			_selectAll();
			break;

		case 0x03: // ctrl + c
		case 0x18: // ctrl + x
			CopyToClipboard();

			if (c == 24)
				_eraseSelection();
			break;

		case 0x16: // ctrl + v
			PasteFromClipboard();
			break;

		case 0x7F: { // ctrl + back
			const auto pContent = GetContent();
			if (pContent != nullptr) {
				const auto& textString = pContent->GetText();
				uint32 startOfWord = Utils::FindStartOfWord(textString, s_caretPosition);
				_eraseText(s_caretPosition - startOfWord);
			}
			}
			break;

		case VK_BACK:
			_eraseText(1);
			break;

		case VK_ESCAPE:
		case VK_RETURN:
			Unfocus();
			break;

		default: {
			std::wstring textString;
			textString += c;
			_insertText(std::move(textString));
			}
		}

		sWndProc.LastMessageHandled = true;
	}

	void EditBox::_onKeyDown(uint32 keyCode) {
		switch (keyCode) {
		case VK_DELETE:
			_eraseText(s_caretPosition, 1);
			break;

		case VK_LEFT: {
			if ((GetKeyState(VK_CONTROL) & 0x8000) != 0x8000) {
				int32 position = s_caretPosition - 1;
				_placeCaret(position < 0 ? 0 : position, true);
			}
			else {
				const auto pContent = GetContent();
				if (pContent != nullptr)
					_placeCaret(Utils::FindStartOfWord(pContent->GetText(), s_caretPosition), true);
			}

			if ((GetKeyState(VK_SHIFT) & 0x8000) != 0x8000)
				_clearSelection();
			}
			break;

		case VK_RIGHT: {
			if ((GetKeyState(VK_CONTROL) & 0x8000) != 0x8000)
				_placeCaret(s_caretPosition + 1, true);
			else {
				const auto pContent = GetContent();
				if (pContent != nullptr)
					_placeCaret(Utils::FindEndOfWord(pContent->GetText(), s_caretPosition), true);
			}

			if ((GetKeyState(VK_SHIFT) & 0x8000) != 0x8000)
				_clearSelection();
			}
			break;

		case VK_HOME:
		case VK_UP: {
			_placeCaret(0, true);

			if ((GetKeyState(VK_SHIFT) & 0x8000) != 0x8000)
				_clearSelection();
			}
			break;

		case VK_END:
		case VK_DOWN: {
			const auto pContent = GetContent();
			if (pContent != nullptr)
				_placeCaret(pContent->GetText().length(), true);

			if ((GetKeyState(VK_SHIFT) & 0x8000) != 0x8000)
				_clearSelection();
			}
			break;

		case VK_TAB:
			return;
		}

		sWndProc.LastMessageHandled = true;
	}

	void EditBox::_onMouseMove(const Utils::Vector2 &position) {
		s_pressedOnFocus = false;
		if (s_activeSelection) {
			const auto pContent = GetContent();
			if (pContent != nullptr) {
				int32 positionOffset = static_cast<int32>(position.x + m_scrollPosition);
				_placeCaret(pContent->XToCP(positionOffset), true);
			}

			sWndProc.LastMessageHandled = true;
		}
	}

	bool EditBox::_scrollTo(uint32 position) {
		// no content label
		const auto pContent = GetContent();
		if (pContent == nullptr)
			return false;

		RECT fullRect = GetFullRect();
		LONG boxWidth = fullRect.right - fullRect.left;

		Utils::Vector2 vIndent = pContent->GetPosition();
		fullRect.left += static_cast<LONG>(vIndent.x);
		fullRect.top += static_cast<LONG>(vIndent.y);
		fullRect.right -= static_cast<LONG>(vIndent.x);
		fullRect.bottom -= static_cast<LONG>(vIndent.y);

		if (fullRect.left >= fullRect.right &&
			fullRect.top >= fullRect.bottom)
			return false; // invalid text area :(

		int32 leftOffset = pContent->CPToX(0);
		int32 rightOffset = pContent->CPToX(position);

		// return value, tells us if it updated the scrolling
		bool scrollUpdate = false;

		// need to scroll left...
		if (fullRect.left + m_scrollPosition > rightOffset) {
			float scrollOffset = static_cast<float>(rightOffset - leftOffset);
			if ((pContent->GetFormatFlags() & DT_CENTER) == DT_CENTER)
				scrollOffset /= 2.0f;

			scrollOffset = floor(scrollOffset + 0.5f);
			scrollUpdate = (scrollOffset < m_scrollPosition);
			if (scrollUpdate)
				m_scrollPosition = scrollOffset;
		}

		// need to scroll right...
		else if (fullRect.right + m_scrollPosition < rightOffset) {
			float scrollOffset = static_cast<float>(rightOffset - leftOffset) - boxWidth + vIndent.x * 2;
			if ((pContent->GetFormatFlags() & DT_CENTER) == DT_CENTER)
				scrollOffset /= 2.0f;

			scrollOffset = floor(scrollOffset + 0.5f);
			scrollUpdate = (scrollOffset > m_scrollPosition);
			if (scrollUpdate)
				m_scrollPosition = scrollOffset;
		}

		if (m_scrollPosition < 0.0f)
			m_scrollPosition = 0.0f;

		return scrollUpdate;
	}

	void EditBox::_placeCaret(uint32 position, bool select) {
		uint32 textLength = 0;
		const auto pContent = GetContent();
		if (pContent != nullptr)
			textLength = pContent->GetText().length();

		if (position > textLength)
			position = textLength;

		// we don't need to move here
		if (s_caretPosition == position)
			return;

		if (!select)
			s_selectPosition1 = position;

		// like wow does -> swaps the direction
		else if (s_swapSelection &&
				 s_caretPosition > position)
			s_selectPosition1 = s_selectPosition2;

		s_caretPosition = position;
		s_selectPosition2 = position;
		s_swapSelection = false;

		_resetCaret();
		_scrollTo(s_caretPosition);
	}

	uint32 EditBox::_insertText(std::wstring textString) {
		uint32 writePosition = _getWritePosition();
		uint32 insertCount = _insertText(writePosition, std::move(textString));
		if (insertCount != 0)
			_placeCaret(writePosition + insertCount);

		return insertCount;
	}

	uint32 EditBox::_insertText(uint32 insertPosition, std::wstring textString) {
		const auto pContent = GetContent();
		if (pContent == nullptr)
			return 0; // cannot insert - no label attached

		// remove all characters which cannot be printed (control characters)
		textString.erase(std::remove_if(textString.begin(), textString.end(), [] (wchar_t c) {
				return (c >= 0x00 && c < 0x20) || c == 0x7F;
			}), textString.end());

		// calculate how many chars it should insert...
		uint32 contentLength = pContent->GetText().length();
		if (insertPosition > contentLength)
			insertPosition = contentLength;

		int32 remainingChars = (m_maxLength - contentLength) + _getSelectionCount();
		uint32 insertCount = (remainingChars < 0 ? 0 : remainingChars);

		uint32 stringLength = textString.length();
		if (insertCount > stringLength)
			insertCount = stringLength;

		if (insertCount > 0) {
			_resetCaret();
			_eraseSelection();

			auto currentText = pContent->GetText();
			currentText.insert(insertPosition, textString, 0, insertCount);
			pContent->SetText(std::move(currentText));
			_notifyContentChangedEvent();
		}

		return insertCount;
	}

	uint32 EditBox::_eraseText(uint32 numChars) {
		uint32 writePosition = _getWritePosition();
		int32 erasePosition = writePosition - numChars;

		// want to erase more than possible?
		if (erasePosition < 0) {
			numChars += erasePosition;
			erasePosition = 0;
		}

		uint32 eraseCount = _eraseText(erasePosition, numChars);
		int32 eraseOffset = writePosition - eraseCount;
		int32 scrollOffset = eraseOffset - 1;

		// adjust scrolling if needed
		if (scrollOffset > 0 && _scrollTo(scrollOffset)) {
			// scroll even more, if we're at the beginning
			scrollOffset = eraseOffset - 4;
			if (scrollOffset > 0)
				_scrollTo(scrollOffset);
		}

		_placeCaret(eraseOffset < 0 ? 0 : eraseOffset);
		return eraseCount;
	}

	uint32 EditBox::_eraseText(uint32 erasePosition, uint32 numChars) {
		if (_hasSelection()) {
			_eraseSelection();
			return 0;
		}

		const auto pContent = GetContent();
		if (pContent == nullptr)
			return 0; // cannot erase - no label attached

		std::wstring textString = pContent->GetText();
		int32 remainingChars = textString.length() - erasePosition;
		uint32 eraseCount = (remainingChars < 0 ? 0 : remainingChars);

		if (eraseCount > numChars)
			eraseCount = numChars;

		if (eraseCount > 0) {
			// if it's a center aligned box we need to adjust the scroll pos a little
			if ((pContent->GetFormatFlags() & DT_CENTER) == DT_CENTER) {
				std::wstring eraseText = pContent->GetText().substr(erasePosition, eraseCount);

				const auto fontProxy = pContent->GetFont();
				if (fontProxy != nullptr) {
					const auto fontObject = fontProxy->GetObject();
					if (fontObject != nullptr) {
						RECT textRect = {0};
						textRect = fontObject->GetTextExtent(eraseText,
							textRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

						m_scrollPosition -= (textRect.right - textRect.left) / 2.0f;
						m_scrollPosition = floor(m_scrollPosition + 0.5f);
						if (m_scrollPosition < 0.0f)
							m_scrollPosition = 0.0f;
					}
				}
			}

			_resetCaret();
			textString.erase(erasePosition, eraseCount);
			pContent->SetText(std::move(textString));
			_notifyContentChangedEvent();
		}

		return eraseCount;
	}

	void EditBox::_notifyPushEvent(Utils::Vector2 *pPosition) {
		s_handleDblClick = !_hasSelection();

		const auto pContent = GetContent();
		if (pContent != nullptr && pPosition != nullptr) {
			uint32 cp = pContent->XToCP(static_cast<int32>(pPosition->x + m_scrollPosition));
			if (cp != s_caretPosition)
				s_handleDblClick = false;

			if (IsFocused()) {
				_placeCaret(cp);
				_clearSelection();
			}
			else {
				// focus would select everything and move
				// caret + scroll pos to the end - back it up
				float scrollPos = m_scrollPosition;

				if (Focus()) {
					s_pressedOnFocus = true;
					m_scrollPosition = scrollPos;

					_placeCaret(cp);
					_clearSelection();
				}
			}
		}

		s_activeSelection = true;
		IPushable::_notifyPushEvent(pPosition);
	}

	void EditBox::_notifyDblClickEvent(Utils::Vector2 *pPosition) {
		const auto pContent = GetContent();
		if (s_handleDblClick && pContent != nullptr) {
			s_swapSelection = true;

			Utils::GetWordPositions(pContent->GetText(), s_caretPosition,
				&s_selectPosition1, &s_selectPosition2);
		}

		s_activeSelection = true;
		IPushable::_notifyDblClickEvent(pPosition);
	}

	void EditBox::_notifyReleaseEvent(Utils::Vector2 *pPosition) {
		s_handleDblClick = true;
		s_activeSelection = false;

		if (s_pressedOnFocus) {
			_selectAll();
			s_pressedOnFocus = false;
		}

		IPushable::_notifyReleaseEvent(pPosition);
	}

	bool EditBox::_notifyFocusBeginEvent() {
		bool result = IFocusable::_notifyFocusBeginEvent();

		if (!result) {
			_resetCaret();
			_placeCaret(UINT_MAX);
			_selectAll();

			const auto pBorder = GetBorder();
			if (pBorder != nullptr)
				pBorder->FadeTo(100, D3DXCOLOR(0.36f, 1.0f, 1.45f, 1.77f));
		}

		return result;
	}

	void EditBox::_notifyFocusEndEvent() {
		const auto pBorder = GetBorder();
		if (pBorder != nullptr)
			pBorder->FadeTo(300, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		IFocusable::_notifyFocusEndEvent();
	}

	void EditBox::BindToLua(const boost::shared_ptr<lua_State> &luaState) {
		luabind::module(luaState.get()) [
			luabind::class_<EditBox,
							luabind::bases<Rectangle, IFocusable, IPushable>,
							boost::shared_ptr<IComponent>>("EditBox")
				.scope [ luabind::def("Create", &EditBox::CreateDefault) ]
				.def("GetCurrentText", &EditBox::GetCurrentText)
				.def_readonly("contentChangedEvent", &EditBox::OnContentChangedLuaWrap)
				.property("content", &EditBox::GetContent, &EditBox::SetContent)
				.property("border", &EditBox::GetBorder, &EditBox::SetBorder)
		];
	}
}
}
