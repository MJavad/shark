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
		
		static std::shared_ptr<ListBoxEntry> Create(std::wstring swText = L"");

		virtual void OnRender(uint32 uTimePassed);
		void RenderCachedFontBatch(const std::shared_ptr<const ID3DSprite> &pSprite) const;
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		virtual void SetWidth(float fWidth);
		virtual void SetHeight(float fHeight);
		
		virtual void SetColor(const Utils::Color &color) {
			Rectangle::SetColor(color);
			m_isSelected ? m_selectedColors.fill(color):
							m_deselectedColors.fill(color);
		}
		
		virtual void SetGradientColors(const std::array<Utils::Color, 4> &gradient) {
			Rectangle::SetGradientColors(gradient);
			m_isSelected ? m_selectedColors = gradient :
							m_deselectedColors = gradient;
		}

		virtual void SetSelectedColor(const Utils::Color &color) {
			m_selectedColors.fill(color);
			if (m_isSelected)
				Rectangle::SetColor(color);
		}
		
		virtual void SetSelectedGradientColors(const std::array<Utils::Color, 4> &gradient) {
			m_selectedColors = gradient;
			if (m_isSelected)
				Rectangle::SetGradientColors(gradient);
		}

		virtual void SetDeselectedColor(const Utils::Color &color) {
			m_deselectedColors.fill(color);
			if (!m_isSelected)
				Rectangle::SetColor(color);
		}
		
		virtual void SetDeselectedGradientColors(const std::array<Utils::Color, 4> &gradient) {
			m_deselectedColors = gradient;
			if (!m_isSelected)
				Rectangle::SetGradientColors(gradient);
		}

		virtual bool GetSelected() const {
			return m_isSelected;
		}

		virtual void SetSelected(bool bSelected) {
			m_isSelected = bSelected;
			Rectangle::SetGradientColors(m_isSelected ?
							m_selectedColors : m_deselectedColors);
		}

		virtual std::shared_ptr<Label> GetCaption() const {
			return m_caption;
		}

		virtual void SetCaption(std::shared_ptr<Label> pCaption) {
			m_caption = std::move(pCaption);
			if (m_caption != nullptr)
				m_caption->SetUIParent(shared_from_this());
		}

		virtual std::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(std::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr) {
				m_border->SetPosition(Utils::Vector2(-1.0f, -1.0f));
				m_border->SetUIParent(shared_from_this());
			}
		}

	protected:
		bool m_isSelected;
		std::shared_ptr<Label> m_caption;
		std::shared_ptr<Rectangle> m_border;
		std::array<Utils::Color, 4> m_deselectedColors, m_selectedColors;
	};
}
}