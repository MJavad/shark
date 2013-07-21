#include "Misc/stdafx.h"
#include "IComponent.h"
#include "Base/D3DManager.h"
#include "Base/Engine.h"

namespace UI {
namespace Components {
	IComponent::IComponent(): m_isVisible(true),
							  m_fadeActive(false),
							  m_prevFade(false),
							  m_moveActive(false),
							  m_prevMove(false),
							  m_colorMod(1.0f, 1.0f, 1.0f, 1.0f)
	{	// register for update...
		m_updateDelegate = sD3DMgr.OnUpdateEvent.connect(
			boost::bind(&IComponent::OnUpdate, this, _1));
	}

	IComponent::~IComponent() {
		sD3DMgr.OnUpdateEvent -= m_updateDelegate;
	}

	void IComponent::OnUpdate(uint32 timePassed) {
		if (m_fadeActive) {
			if (m_fadeTimePassed < m_fadeTime) {
				float fadeTime = static_cast<float>(m_fadeTime);
				float fadeTimePassed = static_cast<float>(m_fadeTimePassed);
				D3DXCOLOR colorChange = ((m_fadeTo - m_fadeSrc) * 1000.0f) / fadeTime;
				SetColorMod((colorChange * fadeTimePassed) / 1000.0f + m_fadeSrc);
			}
			else {
				m_fadeActive = false;
				SetColorMod(m_fadeTo);
			}

			m_isVisible = (m_colorMod.a > 0.0f);
			m_fadeTimePassed += timePassed;
		}

		if (m_moveActive) {
			if (m_moveTimePassed < m_moveTime) {
				Utils::Vector2 position;
				position.x = floor(((m_moveTo.x - m_moveSrc.x) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.x);
				position.y = floor(((m_moveTo.y - m_moveSrc.y) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.y);
				SetPosition(position);
			}
			else {
				m_moveActive = false;
				SetPosition(m_moveTo);
			}

			m_moveTimePassed += timePassed;
		}
	}

	std::list<boost::shared_ptr<IComponent>> IComponent::GetUIHierarchy() {
		std::list<boost::shared_ptr<IComponent>> lstResult;
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent())
			lstResult.push_front(pParent);

		return lstResult;
	}

	std::list<boost::shared_ptr<const IComponent>> IComponent::GetUIHierarchy() const {
		std::list<boost::shared_ptr<const IComponent>> lstResult;
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent())
			lstResult.push_front(pParent);

		return lstResult;
	}

	boost::shared_ptr<ComponentInterface> IComponent::GetInterface() const {
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
		boost::shared_ptr<ComponentInterface> pInterface = GetInterface();
		if (pInterface == nullptr || !pInterface->Visible)
			return false;

		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent()) {
			if (!pParent->GetVisibility())
				return false;
		}

		return true;
	}

	bool IComponent::HasParent(const boost::shared_ptr<const IComponent> &pParent) const {
		for (auto pCurrent = GetUIParent(); pCurrent != nullptr; pCurrent = pCurrent->GetUIParent()) {
			if (pCurrent == pParent)
				return true;
		}

		return false;
	}

	D3DXCOLOR IComponent::CalculateAbsoluteColor(const D3DXCOLOR &color) const {
		D3DXCOLOR result(color);
		for (auto pParent = shared_from_this(); pParent != nullptr; pParent = pParent->GetUIParent()) {
			const auto colorMod = pParent->GetColorMod();
			result.a *= colorMod.a;
			result.r *= colorMod.r;
			result.g *= colorMod.g;
			result.b *= colorMod.b;
		}

		return result;
	}

	std::array<D3DXCOLOR, 4> IComponent::CalculateAbsoluteColor(const std::array<D3DXCOLOR, 4> &gradient) const {
		std::array<D3DXCOLOR, 4> result;
		result[0] = CalculateAbsoluteColor(gradient[0]);
		result[1] = CalculateAbsoluteColor(gradient[1]);
		result[2] = CalculateAbsoluteColor(gradient[2]);
		result[3] = CalculateAbsoluteColor(gradient[3]);
		return result;
	}

	void IComponent::FadeTo(uint32 uFadeTime, const D3DXCOLOR &fadeColor) {
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

		D3DXCOLOR temp(m_fadeTo);
		m_fadeTo = m_fadeSrc;
		m_fadeSrc = temp;
		m_fadeActive = true;
		m_fadeTimePassed = 0;
	}

	void IComponent::MoveTo(uint32 moveTime, const Utils::Vector2 &position) {
		m_moveActive = true;
		m_prevMove = true;
		m_moveTimePassed = 0;
		m_moveTime = moveTime;
		m_moveTo = position;
		m_moveSrc = GetPosition();
	}

	void IComponent::UndoMove() {
		if (!m_prevMove)
			return;

		auto temp = m_moveTo;
		m_moveTo = m_moveSrc;
		m_moveSrc = temp;
		m_moveActive = true;
		m_moveTimePassed = 0;
	}

	void IComponent::Hide(uint32 time) {
		FadeTo(time, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	void IComponent::Show(uint32 time) {
		D3DXCOLOR standard(1.0f, 1.0f, 1.0f, 1.0f);
		D3DXCOLOR highlight(1.25f, 1.25f, 1.45f, 1.1f);

		if (GetColorMod() != standard)
			FadeTo(time, standard);

		else {
			FadeTo(time, highlight);
			sEngine.PulseTimer.AddTimer(time * 2, [=]
				(const Utils::STimerDispatchEvt&) {
					this->FadeTo(uint32(time * 1.5f), standard);
					return TIMER_STOP_EXECUTION;
				});
		}
	}
}
}
