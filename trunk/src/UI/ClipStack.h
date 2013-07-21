#pragma once

namespace UI {
	class ClipStack
	{
	public:
		ClipStack() : m_stack() {}

		void Apply() const;
		void Push(const RECT &rect);
		void Pop() { m_stack.pop(); }
		bool IsEmpty() const { return m_stack.empty(); }
		size_t Size() const { return m_stack.size(); }
		const RECT* Top() const;

		bool PtInClipArea(const POINT &pt) const {
			return m_stack.empty() || (PtInRect(&m_stack.top(), pt) != FALSE);
		}

		void SetRect(const RECT &rect, const boost::function<void ()> &func) {
			Push(rect);
			Apply();
			func();
			Pop();
			Apply();
		}

	protected:
		std::stack<RECT> m_stack;
	};
}
