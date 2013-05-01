#pragma once
#include "UI/ComponentInterface.h"

namespace UI {
	class ComponentInterface;

namespace Components {
	class IComponent abstract : public std::enable_shared_from_this<IComponent>
	{
	public:
		virtual ~IComponent();
		virtual void OnUpdate(uint32 uTimePassed);
		virtual void OnRender(uint32 uTimePassed) = 0;
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) {}

		std::list<std::shared_ptr<IComponent>> GetUIHierarchy();
		std::list<std::shared_ptr<const IComponent>> GetUIHierarchy() const;
		std::shared_ptr<ComponentInterface> GetInterface() const;
		Utils::Vector2 GetScreenPosition() const;
		Utils::Color GetModifiedColor(const Utils::Color &color) const;
		std::array<Utils::Color, 4> GetModifiedColor(const std::array<Utils::Color, 4> &gradient) const;

		bool IsVisible() const;
		bool HasParent(const std::shared_ptr<const IComponent> &pParent) const;

		void StopFade() { m_fadeActive = false; }
		void FadeTo(uint32 uFadeTime, const float4 &fadeColor);
		void UndoFade();
		
		void StopMove() { m_moveActive = false; }
		void MoveTo(uint32 uMoveTime, const Utils::Vector2 &vPosition);
		void UndoMove();

		virtual void Hide(uint32 time = 200);
		virtual void Show(uint32 time = 200);

		virtual bool GetVisibility() const {
			return m_isVisible;
		}

		virtual void SetVisibility(bool bVisible) {
			m_isVisible = bVisible;
		}

		virtual Utils::Vector2 GetPosition() const {
			return m_position;
		}

		virtual void SetPosition(const Utils::Vector2 &vPosition) {
			m_position = vPosition;
		}

		virtual Utils::Vector2 GetChildOffset() const {
			return m_childOffset;
		}

		virtual void SetChildOffset(const Utils::Vector2 &vOffset) {
			m_childOffset = vOffset;
		}

		virtual float4 GetColorMod() const {
			return m_colorMod;
		}

		virtual void SetColorMod(const float4 &fColorMod) {
			m_colorMod = fColorMod;
		}

		virtual std::shared_ptr<IComponent> GetUIParent() const {
			return m_parent.lock();
		}

		virtual void SetUIParent(std::shared_ptr<IComponent> pParent) {
			m_parent = std::move(pParent);
		}

		virtual std::shared_ptr<ComponentInterface> GetClientInterface() const {
			return m_interface.lock();
		}

		virtual void SetClientInterface(std::shared_ptr<ComponentInterface> pInterface) {
			m_interface = std::move(pInterface);
		}

		virtual bool OnParentChanged(const std::shared_ptr<IComponent> &pParent) {
			return true;
		}

		virtual bool OnTabPressed(const std::shared_ptr<IComponent> &pComponent) {
			auto pParent = GetUIParent();
			if (pParent != nullptr)
				return pParent->OnTabPressed(shared_from_this());

			return false;
		}

		virtual void OnParentSizeChanged(float fWidth, float fHeight) {}

	protected:
		// ctor
		IComponent();

		bool m_isVisible;
		float4 m_colorMod;

		bool m_fadeActive, m_prevFade;
		uint32 m_fadeTime, m_fadeTimePassed;
		float4 m_fadeTo, m_fadeSrc;

		bool m_moveActive, m_prevMove;
		uint32 m_moveTime, m_moveTimePassed;
		Utils::Vector2 m_moveTo, m_moveSrc;

		Utils::Vector2 m_position, m_childOffset;

		std::weak_ptr<IComponent> m_parent;
		std::weak_ptr<ComponentInterface> m_interface;

		Utils::SEventDelegate<void (uint32)> m_updateDelegate;

		template <typename T>
		std::shared_ptr<T> get_this() {
			return std::dynamic_pointer_cast<T>(shared_from_this());
		}

		template <typename T>
		std::shared_ptr<const T> get_this() const {
			return std::dynamic_pointer_cast<const T>(shared_from_this());
		}
	};
}
}
