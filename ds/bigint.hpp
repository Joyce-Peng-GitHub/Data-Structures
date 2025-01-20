#ifndef DS_BIGINT_HPP_
#define DS_BIGINT_HPP_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ds {

	// compile-time log2
	namespace {
		constexpr uint8_t log2(uint8_t n) {
			uint8_t result = 0;
			while (n > 1) {
				n /= 2;
				++result;
			}
			return result;
		}
	}

	class bigint {
	public:
		// unit_t is the type of each unit in the bigint
		using unit_t = uint64_t;
		// unit_bits is the number of bits in one unit_t
		uint8_t static constexpr unit_bits = sizeof(unit_t) * 8;
		// log_bits is used to optimize math calculations
		uint8_t static constexpr log_bits = log2(unit_bits);

		inline bigint() = default;
		inline bigint(unit_t x) : m_data(1, x) {}
		inline bigint(const std::string &str) {
			/* TODO: Implement the constructor from string */
		}
		inline bigint(const bigint &) = default;
		inline bigint(bigint &&) = default;

		inline size_t size() const { return size(); }
		inline size_t capacity() const { return m_data.capacity(); }

		inline void reserve(size_t n) { m_data.reserve(n); }
		inline void shrink_to_fit() { m_data.shrink_to_fit(); }

		/**
		 * @brief Removing all trailing 0s or -1s.
		 */
		inline void resize() {
			while (size() && (!m_data.back() ||
							  m_data.back() == static_cast<unit_t>(-1))) {
				m_data.pop_back();
			}
		}
		/**
		 * @brief If @c n > @c size(), expand the number; if @c n < @c size(),
		 * 	remove trailing 0s or -1s until @c n == @c size() or
		 * 	there is no more trailing 0 or -1.
		 */
		inline void resize(size_t n) {
			if (n > size()) {
				m_signed_expand(n);
			} else if (n < size()) {
				while (size() > n &&
					   (!m_data.back() ||
						m_data.back() == static_cast<unit_t>(-1))) {
					m_data.pop_back();
				}
			}
		}

		inline bigint &operator<<=(size_t n) {
			if (m_data.empty()) {
				return *this;
			}
			size_t m = (n >> log_bits); // m = n / unit_bits
			n &= unit_bits - 1;			// n %= unit_bits
			if (!n) {
				/**
				 * A right shift by unit_bits leads to undefined behavior.
				 * So we need to handle separately the case other n == 0
				 * to avoid this problem.
				 */
				m_data.insert(m_data.begin(), m, 0);
				return *this;
			}
			size_t sz = size();
			m_data.resize(sz + m);
			if (m_data[sz - 1] >> (unit_bits - n)) {
				m_data.push_back(m_data[sz - 1] >> (unit_bits - n));
			}
			for (size_t i = sz + m - 1; i != m; --i) {
				m_data[i] = ((m_data[i - m] << n) |
							 (m_data[i - m - 1] >> (unit_bits - n)));
			}
			m_data[m] = (m_data[0] << n);
			for (size_t i = 0; i != m; ++i) {
				m_data[i] = 0;
			}
			return *this;
		}
		inline bigint operator<<(size_t n) const {
			return (bigint(*this) <<= n);
		}

		inline bigint &operator>>=(size_t n) {
			if (m_data.empty()) {
				return *this;
			}
			size_t m = (n >> log_bits); // m = n / unit_bits
			if (m >= size()) {
				m_data.clear();
				return *this;
			}
			n &= unit_bits - 1; // n %= unit_bits
			if (!n) {
				/**
				 * A left shift by unit_bits leads to undefined behavior.
				 * So we need to handle separately the case other n == 0
				 * to avoid this problem.
				 */
				m_data.erase(m_data.begin(), m_data.begin() + m);
				return *this;
			}
			for (size_t i = 0; i + m + 1 != size(); ++i) {
				m_data[i] = ((m_data[i + m] >> n) |
							 (m_data[i + m + 1] << (unit_bits - n)));
			}
			if (m_data.back() >> n) {
				m_data[size() - m - 1] = (m_data.back() >> n);
			} else {
				++m;
			}
			m_data.resize(size() - m);
			return *this;
		}
		inline bigint operator>>(size_t n) const {
			return (bigint(*this) >>= n);
		}

		inline bigint operator~() const {
			bigint res;
			size_t i = res.size() - 1;
			while (~i && m_data[i] == static_cast<uint64_t>(-1)) {
				--i;
			}
			res.m_data.resize(i + 1);
			for (; ~i; --i) {
				res.m_data[i] = ~this->m_data[i];
			}
			return res;
		}

		inline bigint &operator&=(const bigint &other) {
			size_t i = std::min(size(), other.size()) - 1;
			while (~i && !(m_data[i] & other.m_data[i])) {
				--i;
			}
			m_data.resize(i + 1);
			for (; ~i; --i) {
				m_data[i] &= other.m_data[i];
			}
			return *this;
		}
		inline bigint operator&(const bigint &other) const {
			bigint res;
			size_t i = std::min(size(), other.size()) - 1;
			while (~i && !(m_data[i] & other.m_data[i])) {
				--i;
			}
			bigint res;
			res.m_data.resize(i + 1);
			for (; ~i; --i) {
				res.m_data[i] = (m_data[i] & other.m_data[i]);
			}
			return res;
		}
		inline bigint &operator|=(const bigint &other) {
			size_t i = 0;
			for (; i != size() && i != other.size(); ++i) {
				m_data[i] |= other.m_data[i];
			}
			if (i != other.size()) {
				m_data.insert(m_data.end(), other.m_data.begin() + i, other.m_data.end());
			}
			return *this;
		}
		inline bigint operator|(const bigint &other) const {
			bigint res;
			res.m_data.resize(std::max(size(), other.size()));
			size_t i = 0;
			for (; i != size() && i != other.size(); ++i) {
				res.m_data[i] = (m_data[i] | other.m_data[i]);
			}
			for (; i != size(); ++i) {
				res.m_data[i] = m_data[i];
			}
			for (; i < other.size(); ++i) {
				res.m_data[i] = other.m_data[i];
			}
			return res;
		}

		inline bigint &operator^=(const bigint &other) {
			if (size() == other.size()) {
				size_t i = size() - 1;
				while (~i && (m_data[i] == other.m_data[i] ||
							  m_data[i] ^ other.m_data[i] ==
											  static_cast<uint64_t>(-1))) {
					--i;
				}
				m_data.resize(i + 1);
				for (; ~i; --i) {
					m_data[i] ^= other.m_data[i];
				}
			} else if (size() < other.size()) {
				for (size_t i = 0; i != size(); ++i) {
					m_data[i] ^= other.m_data[i];
				}
				m_data.insert(m_data.end(),
							  other.m_data.begin() + size(),
							  other.m_data.end());
			} else {
				for (size_t i = 0; i != other.size(); ++i) {
					m_data[i] ^= other.m_data[i];
				}
			}
			return *this;
		}
		inline bigint operator^(const bigint &other) const {
			bigint res;
			if (size() == other.size()) {
				size_t i = size() - 1;
				while (~i && (m_data[i] == other.m_data[i] ||
							  m_data[i] ^ other.m_data[i] ==
											  static_cast<uint64_t>(-1))) {
					--i;
				}
				res.m_data.resize(i + 1);
				for (; ~i; --i) {
					res.m_data[i] = m_data[i] ^ other.m_data[i];
				}
			} else if (size() < other.size()) {
				res.m_data.resize(other.size());
				for (size_t i = 0; i != size(); ++i) {
					res.m_data[i] = (m_data[i] ^ other.m_data[i]);
				}
				res.m_data.insert(res.m_data.end(),
								  other.m_data.begin() + size(),
								  other.m_data.end());
			} else {
				res.m_data.resize(size());
				for (size_t i = 0; i != other.size(); ++i) {
					res.m_data[i] = (m_data[i] ^ other.m_data[i]);
				}
				res.m_data.insert(res.m_data.end(),
								  m_data.begin() + other.size(),
								  m_data.end());
			}
			return res;
		}

		inline bigint &operator+=(const bigint &other) {
			if (size() < other.size()) {
				m_signed_expand(other.size());
			}
			// size() >= other.size()
			size_t i = 0;
			bool carry = false;
			std::pair<bool, unit_t> p = {false, 0}, q = {false, 0};
			for (; i != other.size(); ++i) {
				p = s_plus_unit(m_data[i], other.m_data[i]);
				q = s_plus_unit(p.second, carry);
				m_data[i] = q.second;
				carry = (p.first || q.first);
			}
			for (; carry && i != size(); ++i) {
				p = s_plus_unit(m_data[i], carry);
				m_data[i] = p.second;
				carry = p.first;
			}
			if (carry) {
				m_data.push_back(1);
			}
			return *this;
		}

		inline bool operator==(const bigint &other) const {
			/* TODO: Implement the == operator for numbers with trailing 0s or -1s */
			return (m_data == other.m_data);
		}

		inline friend std::ostream &testOut(std::ostream &os,
											const bigint &x) {
			if (x.m_data.empty()) {
				return os << 0;
			}
			os << x.m_data.back();
			for (size_t i = x.size() - 2; ~i; --i) {
				os << ' ' << x.m_data[i];
			}
			return os;
		}

	protected:
		std::vector<uint64_t> m_data;

		inline void m_signed_expand(size_t n) { // Suppose n >= size()
			if (m_data.empty()) {
				m_data.resize(n);
			} else if (m_data.back() &
					   (static_cast<uint64_t>(1) << (unit_bits - 1))) {
				m_data.resize(n, static_cast<uint64_t>(-1));
			} else {
				m_data.resize(n);
			}
		}
		inline static std::pair<bool, unit_t> s_plus_unit(unit_t a, unit_t b) {
			unit_t sum = a + b;
			return {(sum < a || sum < b), sum};
		}
	}; // class bigint

} // namespace ds

#endif // DS_BIGINT_HPP_