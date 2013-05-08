#include "Misc/stdafx.h"
#include "IComponent.h"
#include "Base/D3DManager.h"
#include "Base/Engine.h"

namespace UI {
namespace Components {
	IComponent::IComponent(): m_isVisible(true), m_fadeActive(false),
		m_prevFade(false), m_moveActive(false), m_prevMove(false) {
		m_colorMod._1 = 1.0f; m_colorMod._2 = 1.0f;
		m_colorMod._3 = 1.0f; m_colorMod._4 = 1.0f;

		// register for update...
		m_updateDelegate = sD3DMgr->OnUpdateEvent +=
		[this] (uint32 uTimePassed) {
			OnUpdate(uTimePassed);
		};
	}

	IComponent::~IComponent() {
		if (Engine::IsInitialized())
			sD3DMgr->OnUpdateEvent -= m_updateDelegate;
	}

	void IComponent::OnUpdate(uint32 uTimePassed) {
		if (m_fadeActive) {
			if (m_fadeTimePassed < m_fadeTime) {
				m_colorMod._1 = ((m_fadeTo._1 - m_fadeSrc._1) * 1000.0f) / m_fadeTime * m_fadeTimePassed / 1000.0f + m_fadeSrc._1;
				m_colorMod._2 = ((m_fadeTo._2 - m_fadeSrc._2) * 1000.0f) / m_fadeTime * m_fadeTimePassed / 1000.0f + m_fadeSrc._2;
				m_colorMod._3 = ((m_fadeTo._3 - m_fadeSrc._3) * 1000.0f) / m_fadeTime * m_fadeTimePassed / 1000.0f + m_fadeSrc._3;
				m_colorMod._4 = ((m_fadeTo._4 - m_fadeSrc._4) * 1000.0f) / m_fadeTime * m_fadeTimePassed / 1000.0f + m_fadeSrc._4;
			}
			else {
				m_fadeActive = false;
				m_colorMod = m_fadeTo;
			}

			m_isVisible = (m_colorMod._1 > 0.0f);
			m_fadeTimePassed += uTimePassed;
		}

		if (m_moveActive) {
			if (m_moveTimePassed < m_moveTime) {
				m_position.x = floor(((m_moveTo.x - m_moveSrc.x) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.x);
				m_position.y = floor(((m_moveTo.y - m_moveSrc.y) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.y);
			}
			else {
				m_moveActive = false;
				m_position = m_moveTo;
			}

			m_moveTimePassed += uTimePassed;
		}
	}

	std::list<std::shared_ptr<IComponent>> IComponent::GetUIHierarchy() {
		std::list<std::shared_ptr<IComponent>> lstResult;
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent())
			lstResult.push_front(pParent);

		return lstResult;
	}

	std::list<std::shared_ptr<const IComponent>> IComponent::GetUIHierarchy() const {
		std::list<std::shared_ptr<const IComponent>> lstResult;
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent())
			lstResult.push_front(pParent);

		return lstResult;
	}

	std::shared_ptr<ComponentInterface> IComponent::GetInterface() const {
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent()) {
			auto pInterface = pParent->GetClientInterface();
			if (pInterface != nullptr)
				return pInterface;
		}

		return nullptr;
	}

	Utils::Vector2 IComponent::GetScreenPosition() const {
		Utils::Vector2 vCurrent = GetPosition();
		for (auto pParent = GetUIParent(); pParent != nullptr; pParent = pParent->GetUIParent())
			vCurrent += (pParent->GetPosition() + pParent->GetChildOffset());

		return vCurrent;
	}

	bool IComponent::IsVisible() const {
		std::shared_ptr<ComponentInterface> pInterface = GetInterface();
		if (pInterface == nullptr || !pInterface->Visible)
			return false;

		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent()) {
			if (!pParent->GetVisibility())
				return false;
		}

		return true;
	}

	bool IComponent::HasParent(const std::shared_ptr<const IComponent> &pParent) const {
		for (auto pCurrent = GetUIParent(); pCurrent != nullptr; pCurrent = pCurrent->GetUIParent()) {
			if (pCurrent == pParent)
				return true;
		}

		return false;
	}

	Utils::Color IComponent::GetModifiedColor(const Utils::Color &color) const {
		Utils::Color result(color);
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent()) {
			uint32 currA = uint32(result.A * pParent->m_colorMod._1);
			uint32 currR = uint32(result.R * pParent->m_colorMod._2);
			uint32 currG = uint32(result.G * pParent->m_colorMod._3);
			uint32 currB = uint32(result.B * pParent->m_colorMod._4);

			(currA & ~0xFF) != 0 ? result.A = 0xFF : result.A = (currA & 0xFF);
			(currR & ~0xFF) != 0 ? result.R = 0xFF : result.R = (currR & 0xFF);
			(currG & ~0xFF) != 0 ? result.G = 0xFF : result.G = (currG & 0xFF);
			(currB & ~0xFF) != 0 ? result.B = 0xFF : result.B = (currB & 0xFF);
		}

		return result;
	}

	std::array<Utils::Color, 4> IComponent::GetModifiedColor(const std::array<Utils::Color, 4> &gradient) const {
		std::array<Utils::Color, 4> result;
		result[0] = GetModifiedColor(gradient[0]);
		result[1] = GetModifiedColor(gradient[1]);
		result[2] = GetModifiedColor(gradient[2]);
		result[3] = GetModifiedColor(gradient[3]);
		return result;
	}

	void IComponent::FadeTo(uint32 uFadeTime, const float4 &fadeColor) {
		m_fadeActive = true;
		m_prevFade = true;
		m_fadeTimePassed = 0;
		m_fadeTime = uFadeTime;
		m_fadeTo = fadeColor;
		m_fadeSrc = m_colorMod;
	}

	void IComponent::UndoFade() {
		if (!m_prevFade)
			return;

		float4 temp = m_fadeTo;
		m_fadeTo = m_fadeSrc;
		m_fadeSrc = temp;
		m_fadeActive = true;
		m_fadeTimePassed = 0;
	}

	void IComponent::MoveTo(uint32 uMoveTime, const Utils::Vector2 &vPosition) {
		m_moveActive = true;
		m_prevMove = true;
		m_moveTimePassed = 0;
		m_moveTime = uMoveTime;
		m_moveTo = vPosition;
		m_moveSrc = m_position;
	}

	void IComponent::UndoMove() {
		if (!m_prevMove)
			return;

		Utils::Vector2 temp = m_moveTo;
		m_moveTo = m_moveSrc;
		m_moveSrc = temp;
		m_moveActive = true;
		m_moveTimePassed = 0;
	}

	void IComponent::Hide(uint32 time) {
		float4 fInvis = {0.0f, 1.0f, 1.0f, 1.0f};
		FadeTo(time, fInvis);
	}

	void IComponent::Show(uint32 time) {
		float4 fVisib = {1.0f, 1.0f, 1.0f, 1.0f};
		float4 fHighlight = {1.1f, 1.25f, 1.25f, 1.45f};
		float4 fCurrent = GetColorMod();

		if (fVisib._1 == fCurrent._1 && fVisib._2 == fCurrent._2 &&
			fVisib._3 == fCurrent._3 && fVisib._4 == fCurrent._4) {
			FadeTo(time, fHighlight);

			sEngine->PulseTimer.AddTimer(time * 2, [=] (const Utils::STimerDispatchEvt&) {
				this->FadeTo(uint32(time * 1.5f), fVisib);
				return TIMER_STOP_EXECUTION;
			});
		}
		else
			FadeTo(time, fVisib);
	}
}
}
