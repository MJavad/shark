#include "Misc/stdafx.h"
#include "IMovable.h"

namespace UI {
namespace Components {
	void IMovable::OnUpdate(uint32 uTimePassed) {
		if (m_moveActive) {
			if (m_moveTimePassed < m_moveTime) {
				Utils::Vector2 vPosition;
				vPosition.x = floor(((m_moveTo.x - m_moveSrc.x) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.x);
				vPosition.y = floor(((m_moveTo.y - m_moveSrc.y) * 1000.0f) / m_moveTime * m_moveTimePassed / 1000.0f + m_moveSrc.y);
				SetPosition(vPosition);
			}
			else {
				m_moveActive = false;
				SetPosition(m_moveTo);
			}

			m_moveTimePassed += uTimePassed;
		}

		IComponent::OnUpdate(uTimePassed);
	}

	void IMovable::MoveTo(uint32 uMoveTime, const Utils::Vector2 &vPosition) {
		m_moveActive = true;
		m_prevMove = true;
		m_moveTimePassed = 0;
		m_moveTime = uMoveTime;
		m_moveTo = vPosition;
		m_moveSrc = GetPosition();
	}

	void IMovable::UndoMove() {
		if (!m_prevMove)
			return;

		auto temp = m_moveTo;
		m_moveTo = m_moveSrc;
		m_moveSrc = temp;
		m_moveActive = true;
		m_moveTimePassed = 0;
	}
}
}
