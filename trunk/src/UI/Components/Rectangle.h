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

		static std::shared_ptr<Rectangle> Create(float fWidth, float fHeight);

		virtual void OnRender(uint32 uTimePassed);

		virtual float4 GetHorizontalRoundings() const {
			return m_horizRoundings;
		}

		virtual float4 GetVerticalRoundings() const {
			return m_vertRoundings;
		}

		virtual void SetHorizontalRoundings(const float4 &roundings) {
			FlushShadowTexture();
			m_horizRoundings = roundings;
		}

		virtual void SetVerticalRoundings(const float4 &roundings) {
			FlushShadowTexture();
			m_vertRoundings = roundings;
		}

		virtual void SetHorizontalRounding(float fHorizontalRounding) {
			FlushShadowTexture();
			m_horizRoundings._1 = fHorizontalRounding;
			m_horizRoundings._2 = fHorizontalRounding;
			m_horizRoundings._3 = fHorizontalRounding;
			m_horizRoundings._4 = fHorizontalRounding;
		}

		virtual void SetVerticalRounding(float fVerticalRounding) {
			FlushShadowTexture();
			m_vertRoundings._1 = fVerticalRounding;
			m_vertRoundings._2 = fVerticalRounding;
			m_vertRoundings._3 = fVerticalRounding;
			m_vertRoundings._4 = fVerticalRounding;
		}

		virtual void SetColor(const Utils::Color &color) {
			FlushShadowTexture();
			m_colors.fill(color);
		}

		virtual void SetGradientColors(std::array<Utils::Color, 4> gradient) {
			FlushShadowTexture();
			m_colors = std::move(gradient);
		}

		virtual std::array<Utils::Color, 4> GetGradientColors() const {
			return m_colors;
		}

		virtual void SetDropShadow(bool bDropShadow) {
			m_dropShadow = bDropShadow;
			if (!bDropShadow)
				FlushShadowTexture();
		}

		virtual bool GetDropShadow() const {
			return m_dropShadow;
		}

		virtual void SetShadowDirection(const Utils::Vector2 &vShadowDirection) {
			m_shadowDirection = vShadowDirection;
		}

		virtual Utils::Vector2 GetShadowDirection() const {
			return m_shadowDirection;
		}

		virtual void SetDimensions(std::array<Utils::Vector2, 4> dimensions);

		virtual std::array<Utils::Vector2, 4> GetDimensions() const {
			return m_dimensions;
		}

		virtual void SetWidth(float fWidth) {
			FlushShadowTexture();
			IRectComponent::SetWidth(fWidth);
			m_dimensions[0].x = 0; m_dimensions[1].x = fWidth;
			m_dimensions[3].x = 0; m_dimensions[2].x = fWidth;
		}

		virtual void SetHeight(float fHeight) {
			FlushShadowTexture();
			IRectComponent::SetHeight(fHeight);
			m_dimensions[0].y = 0; m_dimensions[3].y = fHeight;
			m_dimensions[1].y = 0; m_dimensions[2].y = fHeight;
		}

		void CreateShadowTexture();

		void FlushShadowTexture() {
			m_shadowTexture.reset();
		}

	protected:
		bool m_dropShadow;
		Utils::Vector2 m_shadowDirection;
		std::shared_ptr<D3DTexture> m_shadowTexture;

		float4 m_horizRoundings;
		float4 m_vertRoundings;
		std::array<Utils::Color, 4> m_colors;
		std::array<Utils::Vector2, 4> m_dimensions;

		Utils::SEventDelegate<void ()> m_lostDevice;
		Utils::SEventDelegate<void ()> m_changeDevice;
	};
}
}
