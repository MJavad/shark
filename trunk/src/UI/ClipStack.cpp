#include "Misc/stdafx.h"
#include "ClipStack.h"
#include "Base/D3DManager.h"

namespace UI {
	void ClipStack::Apply() const {
		if (sD3DMgr.IsCreated()) 
			sD3DMgr.GetRenderTarget()->SetClippingArea(Top());
	}

	void ClipStack::Push(const RECT &rect) {
		if (m_stack.empty())
			return m_stack.push(rect);

		RECT newTop = {0};
		RECT curTop = m_stack.top();
		if (IntersectRect(&newTop, &curTop, &rect) == FALSE)
			newTop = curTop;

		m_stack.push(newTop);
	}

	const RECT* ClipStack::Top() const {
		if (!m_stack.empty())
			return &m_stack.top();
		return nullptr;
	}
}
