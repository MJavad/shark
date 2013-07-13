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

		static std::shared_ptr<Rectangle> Create(float width, float height);

		virtual void OnRender(uint32 timePassed);

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

		virtual void SetHorizontalRounding(float horizontalRounding) {
			FlushShadowTexture();
			m_horizRoundings._1 = horizontalRounding;
			m_horizRoundings._2 = horizontalRounding;
			m_horizRoundings._3 = horizontalRounding;
			m_horizRoundings._4 = horizontalRounding;
		}

		virtual void SetVerticalRounding(float verticalRounding) {
			FlushShadowTexture();
			m_vertRoundings._1 = verticalRounding;
			m_vertRoundings._2 = verticalRounding;
			m_vertRoundings._3 = verticalRounding;
			m_vertRoundings._4 = verticalRounding;
		}

		virtual void SetColor(const D3DXCOLOR &color) {
			FlushShadowTexture();
			m_colors.fill(color);
		}

		virtual void SetGradientColors(std::array<D3DXCOLOR, 4> gradient) {
			FlushShadowTexture();
			m_colors = std::move(gradient);
		}

		virtual std::array<D3DXCOLOR, 4> GetGradientColors() const {
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

		virtual void SetWidth(float width) {
			FlushShadowTexture();
			IRectComponent::SetWidth(width);
			m_dimensions[0].x = 0; m_dimensions[1].x = width;
			m_dimensions[3].x = 0; m_dimensions[2].x = width;
		}

		virtual void SetHeight(float height) {
			FlushShadowTexture();
			IRectComponent::SetHeight(height);
			m_dimensions[0].y = 0; m_dimensions[3].y = height;
			m_dimensions[1].y = 0; m_dimensions[2].y = height;
		}

		void CreateShadowTexture();

		void FlushShadowTexture() {
			m_shadowTexture.reset();
		}

	private:
		bool m_dropShadow;
		Utils::Vector2 m_shadowDirection;
		std::shared_ptr<D3DTexture> m_shadowTexture;

		float4 m_horizRoundings;
		float4 m_vertRoundings;
		std::array<D3DXCOLOR, 4> m_colors;
		std::array<Utils::Vector2, 4> m_dimensions;

		Utils::SEventDelegate<void ()> m_lostDevice;
		Utils::SEventDelegate<void ()> m_changeDevice;
	};
}
}
