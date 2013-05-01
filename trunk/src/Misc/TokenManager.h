#pragma once

namespace Utils
{
	class TokenManager
	{
	public:
		TokenManager() : m_curMaxToken(0) {}

		TokenManager(TokenManager &&other) :
			m_curMaxToken(other.m_curMaxToken),
			m_freeTokens(std::move(other.m_freeTokens)) {
			other.m_curMaxToken = 0;
		}

		TokenManager& operator = (TokenManager &&other) {
			m_curMaxToken = other.m_curMaxToken;
			m_freeTokens = std::move(other.m_freeTokens);

			other.m_curMaxToken = 0;
			return *this;
		}

		uint64 get() {
			if (m_freeTokens.empty())
				return ++m_curMaxToken;

			auto itr = m_freeTokens.begin();
			uint64 curToken = *itr;
			m_freeTokens.erase(itr);
			return curToken;
		}

		void remove(const uint64 &token) {
			if (token != 0) {
				if (token == m_curMaxToken)
					--m_curMaxToken;

				else if (token < m_curMaxToken)
					m_freeTokens.insert(token);
			}
		}

		bool is_free(const uint64 &token) const {
			return token > m_curMaxToken ||
				m_freeTokens.find(token) != m_freeTokens.end();
		}

		void reset() {
			m_curMaxToken = 0;
			m_freeTokens.clear();
		}

	protected:
		uint64 m_curMaxToken;
		std::set<uint64> m_freeTokens;
	};
}
