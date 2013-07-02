#pragma once
#include "Label.h"
#include "Rectangle.h"
#include "IFocusable.h"
#include "IPushable.h"

namespace UI {
namespace Components {
	class EditBox : public Rectangle, public IFocusable, public IPushable
	{
	public:
		EditBox() : m_scrollPosition(0.0f), m_maxLength(80) {}

		static std::shared_ptr<EditBox> Create(bool centerAlign = false,
			float fWidth = 200.0f, float fHeight = 25.0f,
			float fHorizontalRounding = 6.0f, float fVerticalRounding = 3.0f);

		virtual void OnRender(uint32 uTimePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
			return IFocusable::OnTabPressed(pComponent);
		}

		virtual void SetWidth(float fWidth);
		virtual void SetHeight(float fHeight);

		virtual std::shared_ptr<Label> GetContent() const {
			return m_content;
		}

		virtual void SetContent(std::shared_ptr<Label> pContent) {
			m_content = std::move(pContent);
			if (m_content != nullptr)
				m_content->SetUIParent(shared_from_this());
		}

		virtual std::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(std::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr)
				m_border->SetUIParent(shared_from_this());
		}

		std::wstring GetCurrentSelection() const;

		Utils::Event<void (const std::shared_ptr<EditBox>&)> OnContentChangedEvent;

	protected:
		virtual void _notifyPushEvent(Utils::Vector2 *pvPosition);
		virtual void _notifyDblClickEvent(Utils::Vector2 *pvPosition);
		virtual void _notifyReleaseEvent(Utils::Vector2 *pvPosition);

		virtual bool _notifyFocusStartEvent();
		virtual void _notifyFocusEndEvent();

		virtual void _notifyContentChangedEvent() {
			LOG_DEBUG("%08X: ContentChanged triggered.", this);
			OnContentChangedEvent(get_this<EditBox>());
		}

	private:
		static bool s_renderCaret;
		static uint32 s_caretPosition;
		static uint32 s_caretTimer;

		static bool s_activeSelection;
		static bool s_swapSelection;
		static bool s_handleDblClick;
		static uint32 s_selectPosition1, s_selectPosition2;

		uint32 m_maxLength;
		float m_scrollPosition;
		std::shared_ptr<Label> m_content;
		std::shared_ptr<Rectangle> m_border;

		void _onChar(wchar_t c);
		void _onKeyDown(int key);
		void _onMouseMove(const Utils::Vector2 &vPosition);

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

		uint32 _insertText(std::wstring swText);
		uint32 _insertText(uint32 insertPosition, std::wstring swText);

		uint32 _eraseText(uint32 numChars);
		uint32 _eraseText(uint32 erasePosition, uint32 numChars);
	};
}
}
