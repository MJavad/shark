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
		EditBox() : m_scrollPosition(0) {}

		static std::shared_ptr<EditBox> Create(float fWidth = 200.0f, float fHeight = 25.0f,
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

		void InsertText(std::wstring swText);
		void EraseText(uint32 numChars);

	protected:
		static bool s_renderCaret;
		static uint32 s_caretPosition;
		static uint32 s_caretTimer;
		static bool s_activeSelection;
		static bool s_swapSelection;

		int32 m_scrollPosition;
		std::shared_ptr<Label> m_content;
		std::shared_ptr<Rectangle> m_border;

		void _onChar(wchar_t c);
		void _onKeyDown(int key);
		void _onMouseMove(const Utils::Vector2 &vPosition);

		virtual void OnPushRequest(const Utils::Vector2 &vPosition);
		virtual void OnClickRequest(const Utils::Vector2 &vPosition);
		virtual void OnReleaseRequest(const Utils::Vector2 &vPosition);

		virtual bool OnFocusStartRequest();
		virtual void OnFocusEndRequest();
	};
}
}
