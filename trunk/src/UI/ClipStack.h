#pragma once

namespace UI {
	class ClipStack
	{
	public:
		ClipStack() : m_stack() {}

		void apply() const;
		void push(const RECT &rect);
		void pop() { m_stack.pop(); }
		bool empty() const { return m_stack.empty(); }
		size_t size() const { return m_stack.size(); }
		const RECT* top() const;

		bool PtInClipArea(const POINT &pt) const {
			return m_stack.empty() || (PtInRect(&m_stack.top(), pt) != FALSE);
		}

		void SetRect(const RECT &rect, const std::function<void ()> &func) {
			push(rect);
			apply();
			func();
			pop();
			apply();
		}

	protected:
		std::stack<RECT> m_stack;
	};
}
