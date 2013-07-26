#pragma once
#include "IRectComponent.h"

namespace UI {
namespace Components {
	class IPushable abstract : public virtual IRectComponent
	{
	public:
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		static boost::shared_ptr<IPushable> GetActiveClick() {
			return s_activeClick.lock();
		}

		bool IsPressed() const {
			return GetActiveClick().get() == this;
		}

		// lua wrapper
		Utils::Event<void ()> OnClickLuaWrap;
		Utils::Event<void ()> OnDblClickLuaWrap;
		Utils::Event<void ()> OnPushLuaWrap;
		Utils::Event<void ()> OnReleaseLuaWrap;

		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnDblClickEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnPushEvent;
		Utils::Event<void (const boost::shared_ptr<IPushable>&, Utils::Vector2*)> OnReleaseEvent;

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		IPushable() : m_lastClick(0) {}

		virtual void _notifyClickEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Click triggered.", this);
			OnClickLuaWrap();
			OnClickEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyDblClickEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: DblClick triggered.", this);
			OnDblClickLuaWrap();
			OnDblClickEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyPushEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Push triggered.", this);
			OnPushLuaWrap();
			OnPushEvent(get_this<IPushable>(), pPosition);
		}

		virtual void _notifyReleaseEvent(Utils::Vector2 *pPosition) {
			LOG_DEBUG("%08X: Release triggered.", this);
			OnReleaseLuaWrap();
			OnReleaseEvent(get_this<IPushable>(), pPosition);
		}

	private:
		uint32 m_lastClick;
		static boost::weak_ptr<IPushable> s_activeClick;
	};
}
}
