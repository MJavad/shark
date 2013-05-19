#pragma once

namespace Utils {
	class ByteBuffer {
	public:
		ByteBuffer() :
			m_storage(), m_rpos(0), m_wpos(0) {}

		ByteBuffer(size_t uSize, byte bData = 0) :
			m_storage(), m_rpos(0), m_wpos(0) {
			m_storage.resize(uSize, bData);
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
		ByteBuffer& operator << (const T &tData) {
			put(tData);
			return *this;
		}

		template <typename T>
		ByteBuffer& operator >> (T &tData) {
			tData = get<T>();
			return *this;
		}

		template <typename T>
		const ByteBuffer& operator >> (T &tData) const {
			tData = get<T>();
			return *this;
		}

		bool operator == (const ByteBuffer &other) const {
			return m_storage == other.m_storage;
		}

		bool operator != (const ByteBuffer &other) const {
			return m_storage != other.m_storage;
		}

		std::vector<byte>& to_vector() {
			return m_storage;
		}

		const std::vector<byte>& to_vector() const {
			return m_storage;
		}

		byte& operator [] (const uint32 &uPos) {
			return m_storage[uPos];
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

		void resize(size_t uSize, byte bData = 0) {
			m_storage.resize(uSize, bData);
			if (m_rpos > uSize)
				m_rpos = uSize;
			if (m_wpos > uSize)
				m_wpos = uSize;
		}

		void reserve(size_t uSize) {
			m_storage.reserve(uSize);
		}

		void put_array(const byte *pbData, size_t uSize) {
			put_array(pbData, uSize, m_rpos);
			m_rpos += uSize;
		}

		void put_array(const byte *pbData, size_t uSize, uint32 uPos);
		void erase(uint32 uPos, size_t uSize);

		template <size_t _Size>
		void put_array_s(const byte (&bData)[_Size]) {
			put_array(bData, _Size);
		}

		template <size_t _Size>
		void put_array_s(const byte (&bData)[_Size], uint32 uPos) {
			put_array(bData, _Size, uPos);
		}

		template <typename T>
		void put(const T &tData) {
			put(tData, m_wpos);
			m_wpos += sizeof(T);
		}

		template <typename T>
		void put(const T &tData, const uint32 &uPos) {
			if (m_storage.size() < uPos + sizeof(T))
				m_storage.resize(uPos + sizeof(T));
			*reinterpret_cast<T*>(&m_storage[uPos]) = tData;
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
		T& get(const uint32 &uPos) {
			if (m_storage.size() >= uPos + sizeof(T))
				return *reinterpret_cast<T*>(&m_storage[uPos]);
			throw std::out_of_range("[ByteBuffer] Bad get: Storage access violation.");
		}

		template <typename T>
		const T& get(const uint32 &uPos) const {
			if (m_storage.size() >= uPos + sizeof(T))
				return *reinterpret_cast<const T*>(&m_storage[uPos]);
			throw std::out_of_range("[ByteBuffer] Bad get: Storage access violation.");
		}

		template<>
		void put(const ByteBuffer &tData) {
			put_array(tData.data(), tData.size());
		}

		template<>
		void put(const ByteBuffer &tData, const uint32 &uPos) {
			put_array(tData.data(), tData.size(), uPos);
		}

	protected:
		uint32 m_wpos;
		mutable uint32 m_rpos;
		std::vector<byte> m_storage;
	};
}
