#pragma once

namespace Utils {
	template <typename InputIterator, typename UnaryFunction>
	UnaryFunction WeakForEach(InputIterator first, InputIterator last, UnaryFunction f) {
		for (; first != last; ++first) {
			if (!first->expired())
				f(first->lock());
		}

		return f;
	}
}
