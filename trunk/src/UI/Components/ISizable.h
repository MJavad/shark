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

		virtual void OnRender(uint32 uTimePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool IsSizing() const {
			return GetActiveSizer().get() == this;
		}

		static void ResetActiveSizer() {
			s_activeSizer.reset();
		}

		static std::shared_ptr<ISizable> GetActiveSizer() {
			return s_activeSizer.lock();
		}

		void StartSizing(const Utils::Vector2 &vSizeVector) {
			s_activeSizer = get_this<ISizable>();
			s_sizeVector = vSizeVector;
		}

		bool GetGripVisibility() const {
			return m_isGripVisible;
		}

		void SetGripVisibility(bool bVisible) {
			m_isGripVisible = bVisible;
		}

		bool GetSizeLimited() const {
			return m_isSizeLimited;
		}

		void SetSizeLimited(bool bLimit) {
			m_isSizeLimited = bLimit;
		}

		Utils::Vector2 GetMinSize() const {
			return m_minSize;
		}

		void SetMinSize(const Utils::Vector2 &vMin) {
			m_minSize = vMin;
		}

		Utils::Vector2 GetMaxSize() const {
			return m_maxSize;
		}

		void SetMaxSize(const Utils::Vector2 &vMax) {
			m_maxSize = vMax;
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

		// returning true = no size
		Utils::Event<bool (const std::shared_ptr<ISizable>&, Utils::Vector2*)> OnResizeStartEvent;
		Utils::Event<bool (const std::shared_ptr<ISizable>&, Utils::Vector2*)> OnResizeEndEvent;
		Utils::Event<bool (const std::shared_ptr<ISizable>&, float, float)> OnResizeEvent;

	protected:
		ISizable();

		virtual bool OnResizeStartEventNotify(Utils::Vector2 *pvPosition) {
			return OnResizeStartEvent(get_this<ISizable>(), pvPosition);
		}

		virtual bool OnResizeEndEventNotify(Utils::Vector2 *pvPosition) {
			return OnResizeEndEvent(get_this<ISizable>(), pvPosition);
		}

		virtual bool OnResizeEventNotify(float fWidth, float fHeight) {
			return OnResizeEvent(get_this<ISizable>(), fWidth, fHeight);
		}

	private:
		bool m_isGripVisible;
		bool m_isHovered;

		bool m_isSizeLimited;
		Utils::Vector2 m_minSize, m_maxSize;

		std::shared_ptr<D3DTexture> m_resizeTexture;
		std::shared_ptr<D3DTexture> m_resizeTextureHover;

		static std::weak_ptr<ISizable> s_activeSizer;
		static Utils::Vector2 s_sizeVector;
	};
}
}
