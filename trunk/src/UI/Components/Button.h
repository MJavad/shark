#pragma once
#include "Rectangle.h"
#include "Label.h"
#include "IHoverable.h"
#include "IFocusable.h"
#include "IPushable.h"
#include "Texture.h"
#include "Base/D3DManager.h"
#include "UI/GUIManager.h"

namespace UI {
namespace Components {
	class Button : public Rectangle, public IHoverable, public IFocusable, public IPushable
	{
	public:
		Button() : m_renderBackground(true) {}

		// wrapper for luabind ctor
		static boost::shared_ptr<Button> CreateDefault() {
			const auto pButton = Create();
			sGUIMgr.GetInterface()->PushControl(pButton);
			return pButton;
		}

		static boost::shared_ptr<Button> Create(
			std::wstring textString = L"Default Button",
			float width = 100.0f, float height = 20.0f,
			float horizontalRounding = 6.0f, float verticalRounding = 3.0f);

		virtual void OnRender(uint32 timePassed);
		virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual bool OnTabPressed(const boost::shared_ptr<IComponent> &pComponent) {
			return IFocusable::OnTabPressed(pComponent);
		}
		
		virtual void SetWidth(float width);
		virtual void SetHeight(float height);

		virtual boost::shared_ptr<Label> GetCaption() const {
			return m_caption;
		}

		virtual void SetCaption(boost::shared_ptr<Label> pCaption) {
			m_caption = std::move(pCaption);
			if (m_caption != nullptr)
				m_caption->SetUIParent(shared_from_this());
		}

		virtual boost::shared_ptr<Rectangle> GetBorder() const {
			return m_border;
		}

		virtual void SetBorder(boost::shared_ptr<Rectangle> pBorder) {
			m_border = std::move(pBorder);
			if (m_border != nullptr)
				m_border->SetUIParent(shared_from_this());
		}

		virtual std::list<boost::shared_ptr<Texture>> GetTextures() const {
			return m_textures;
		}

		virtual void AddTexture(boost::shared_ptr<Texture> pTexture) {
			pTexture->SetUIParent(shared_from_this());
			m_textures.push_back(std::move(pTexture));
		}

		virtual void RemoveTexture(const boost::shared_ptr<Texture> &pTexture) {
			m_textures.remove(pTexture);
		}

		bool GetRenderBackground() const {
			return m_renderBackground;
		}

		void SetRenderBackground(bool renderBackground) {
			m_renderBackground = renderBackground;
		}

		static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

	protected:
		virtual void _notifyPushEvent(Utils::Vector2 *pPosition);
		virtual void _notifyClickEvent(Utils::Vector2 *pPosition);
		virtual void _notifyReleaseEvent(Utils::Vector2 *pPosition);

		virtual bool _notifyHoverBeginEvent() {
			return IHoverable::_notifyHoverBeginEvent() || IsPressed();
		}

		virtual bool _notifyHoverEndEvent() {
			return IHoverable::_notifyHoverEndEvent() || IsPressed();
		}

		virtual bool _notifyFocusBeginEvent();
		virtual void _notifyFocusEndEvent();

	private:
		bool m_renderBackground;
		boost::shared_ptr<Label> m_caption;
		boost::shared_ptr<Rectangle> m_border;
		std::list<boost::shared_ptr<Texture>> m_textures;
	};
}
}
