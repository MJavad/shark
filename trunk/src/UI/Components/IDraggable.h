#pragma once
#include "IComponent.h"
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IDraggable abstract : public virtual IRectComponent
	{
	public:
		virtual ~IDraggable() {
			if (IsDragged())
				s_activeDrag.reset();
		}

		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool IsDragged() const {
			return GetActiveDrag().get() == this;
		}

		static void ClearDrag() {
			s_activeDrag.reset();
		}

		static std::shared_ptr<IDraggable> GetActiveDrag() {
			return s_activeDrag.lock();
		}

		void StartDrag(const Utils::Vector2 &vDragVector) {
			s_activeDrag = get_this<IDraggable>();
			s_dragVector = vDragVector;
		}

		// returning true = no drag
		Utils::Event<bool (Utils::Vector2*)> OnDragStartEvent;
		Utils::Event<bool (Utils::Vector2*)> OnDragEndEvent;
		Utils::Event<bool (Utils::Vector2*)> OnDragMoveEvent;

	protected:
		static std::weak_ptr<IDraggable> s_activeDrag;
		static Utils::Vector2 s_dragVector;

		virtual bool OnDragStartRequest(Utils::Vector2 *pvPosition) {
			return OnDragStartEvent(pvPosition);
		}

		virtual bool OnDragEndRequest(Utils::Vector2 *pvPosition) {
			return OnDragEndEvent(pvPosition);
		}

		virtual bool OnDragMoveRequest(Utils::Vector2 *pvPosition) {
			return OnDragMoveEvent(pvPosition);
		}
	};
}
}
