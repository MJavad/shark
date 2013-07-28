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

namespace Utils {
	class ByteBuffer {
	public:
		ByteBuffer() :
			m_storage(), m_rpos(0), m_wpos(0) {}

		ByteBuffer(size_t size, byte data = 0) :
			m_storage(), m_rpos(0), m_wpos(0) {
			m_storage.resize(size, data);
		}

		ByteBuffer(ByteBuffer &&other) :
			m_rpos(other.m_rpos),
			m_wpos(other.m_wpos),
			m_storage(std::move(other.m_storage)) {
			other.m_rpos = 0; other.m_wpos = 0;
		}

		ByteBuffer& operator = (ByteBuffer &&other) {
			m_rpos = other.m_rpos;
			m_wpos = other.m_wpos;
			m_storage = std::move(other.m_storage);

			other.m_rpos = 0;
			other.m_wpos = 0;
			return *this;
		}

		template <typename T>
		ByteBuffer(const std::vector<T> &storage) : m_rpos(0), m_wpos(0) {
			put_array((const byte*) storage.data(), storage.size() * sizeof(T));
		}

		template <typename T>
		ByteBuffer& operator << (const T &data) {
			put(data);
			return *this;
		}

		template <typename T>
		ByteBuffer& operator >> (T &data) {
			data = get<T>();
			return *this;
		}

		template <typename T>
		const ByteBuffer& operator >> (T &data) const {
			data = get<T>();
			return *this;
		}

		bool operator == (const ByteBuffer &other) const {
			return m_storage == other.m_storage;
		}

		bool operator != (const ByteBuffer &other) const {
			return m_storage != other.m_storage;
		}

		operator std::vector<byte>&() {
			return m_storage;
		}

		operator const std::vector<byte>&() const {
			return m_storage;
		}

		byte& operator [] (const uint32 &pos) {
			return m_storage[pos];
		}

		size_t size() const {
			return m_storage.size();
		}

		size_t max_size() const {
			return m_storage.max_size();
		}

		size_t capacity() const {
			return m_storage.capacity();
		}

		bool empty() const {
			return m_storage.empty();
		}

		byte* data() {
			return m_storage.data();
		}

		const byte* data() const {
			return m_storage.data();
		}

		void clear() {
			m_rpos = 0;
			m_wpos = 0;
			m_storage.clear();
		}

		void reset() {
			m_rpos = 0;
			m_wpos = 0;
		}

		void resize(size_t size, byte data = 0) {
			m_storage.resize(size, data);
			if (m_rpos > size)
				m_rpos = size;
			if (m_wpos > size)
				m_wpos = size;
		}

		void reserve(size_t size) {
			m_storage.reserve(size);
		}

		void put_array(const byte *data, size_t size) {
			put_array(data, size, m_rpos);
			m_rpos += size;
		}

		void put_array(const byte *data, size_t size, uint32 pos);
		void erase(uint32 pos, size_t size);

		template <size_t _Size>
		void put_array_s(const byte (&data)[_Size]) {
			put_array(data, _Size);
		}

		template <size_t _Size>
		void put_array_s(const byte (&data)[_Size], uint32 pos) {
			put_array(data, _Size, pos);
		}

		template <typename T>
		void put(const T &data) {
			put(data, m_wpos);
			m_wpos += sizeof(T);
		}

		template <typename T>
		void put(const T &data, const uint32 &pos) {
			if (m_storage.size() < pos + sizeof(T))
				m_storage.resize(pos + sizeof(T));
			*reinterpret_cast<T*>(&m_storage[pos]) = data;
		}

		template <typename T>
		T& get() {
			T& tResult = get<T>(m_rpos);
			m_rpos += sizeof(T);
			return tResult;
		}

		template <typename T>
		const T& get() const {
			const T& tResult = get<T>(m_rpos);
			m_rpos += sizeof(T);
			return tResult;
		}

		template <typename T>
		T& get(const uint32 &pos) {
			if (m_storage.size() >= pos + sizeof(T))
				return *reinterpret_cast<T*>(&m_storage[pos]);
			throw std::out_of_range("[ByteBuffer] Bad get: Storage access violation.");
		}

		template <typename T>
		const T& get(const uint32 &pos) const {
			if (m_storage.size() >= pos + sizeof(T))
				return *reinterpret_cast<const T*>(&m_storage[pos]);
			throw std::out_of_range("[ByteBuffer] Bad get: Storage access violation.");
		}

		template<>
		void put(const ByteBuffer &data) {
			put_array(data.data(), data.size());
		}

		template<>
		void put(const ByteBuffer &data, const uint32 &pos) {
			put_array(data.data(), data.size(), pos);
		}

	protected:
		uint32 m_wpos;
		mutable uint32 m_rpos;
		std::vector<byte> m_storage;
	};
}
