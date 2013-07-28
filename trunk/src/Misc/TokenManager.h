/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
