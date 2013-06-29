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
		Utils::Event<bool (const std::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragStartEvent;
		Utils::Event<bool (const std::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragEndEvent;
		Utils::Event<bool (const std::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragMoveEvent;

	protected:
		virtual bool _notifyDragStartEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: DragStart triggered.", this);
			return OnDragStartEvent(get_this<IDraggable>(), pvPosition);
		}

		virtual bool _notifyDragEndEvent(Utils::Vector2 *pvPosition) {
			LOG_DEBUG("%08X: DragEnd triggered.", this);
			return OnDragEndEvent(get_this<IDraggable>(), pvPosition);
		}

		virtual bool _notifyDragMoveEvent(Utils::Vector2 *pvPosition) {
			return OnDragMoveEvent(get_this<IDraggable>(), pvPosition);
		}

	private:
		static std::weak_ptr<IDraggable> s_activeDrag;
		static Utils::Vector2 s_dragVector;
	};
}
}
