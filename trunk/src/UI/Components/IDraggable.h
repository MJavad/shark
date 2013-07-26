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
			auto activeDrag = GetActiveDrag();
			s_activeDrag.reset();

			if (activeDrag != nullptr)
				activeDrag->OnDragEndEvent(activeDrag, nullptr);
		}

		static boost::shared_ptr<IDraggable> GetActiveDrag() {
			return s_activeDrag.lock();
		}

		void StartDrag(const Utils::Vector2 &dragVector) {
			s_activeDrag = get_this<IDraggable>();
			s_dragVector = dragVector;
		}

		// lua wrapper
		Utils::Event<void ()> OnDragBeginLuaWrap;
		Utils::Event<void ()> OnDragEndLuaWrap;
		Utils::Event<void ()> OnDragMoveLuaWrap;

		// returning true = no drag
		Utils::Event<bool (const boost::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragBeginEvent;
		Utils::Event<bool (const boost::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragEndEvent;
		Utils::Event<bool (const boost::shared_ptr<IDraggable>&, Utils::Vector2*)> OnDragMoveEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IDraggable() {}

		virtual bool _notifyDragBeginEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: DragStart triggered.", this);
			OnDragBeginLuaWrap();
			return OnDragBeginEvent(get_this<IDraggable>(), pPosition);
		}

		virtual bool _notifyDragEndEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: DragEnd triggered.", this);
			OnDragEndLuaWrap();
			return OnDragEndEvent(get_this<IDraggable>(), pPosition);
		}

		virtual bool _notifyDragMoveEvent(Utils::Vector2 *pPosition) {
			OnDragMoveLuaWrap();
			return OnDragMoveEvent(get_this<IDraggable>(), pPosition);
		}

	private:
		static boost::weak_ptr<IDraggable> s_activeDrag;
		static Utils::Vector2 s_dragVector;
	};
}
}
