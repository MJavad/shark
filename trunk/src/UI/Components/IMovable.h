#pragma once
#include "IComponent.h"

namespace UI {
namespace Components {
	class IMovable abstract : public virtual IComponent
	{
	public:
		virtual void OnUpdate(uint32 uTimePassed);

		void StopMove() { m_moveActive = false; }
		void MoveTo(uint32 uMoveTime, const Utils::Vector2 &vPosition);
		void UndoMove();

	protected:
		IMovable() : m_moveActive(false),
					 m_prevMove(false) {}

	private:
		bool m_moveActive, m_prevMove;
		uint32 m_moveTime, m_moveTimePassed;
		Utils::Vector2 m_moveTo, m_moveSrc;
	};
}
}
