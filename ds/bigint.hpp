#ifndef DS_BIGINT_HPP_
#define DS_BIGINT_HPP_

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ds {

	// compile-time log2
	static constexpr uint8_t log2(uint8_t n) {
		uint8_t result = 0;
		while (n > 1) {
			n /= 2;
			++result;
		}
		return result;
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
		inline explicit bigint(const unit_t &x) : m_data(1, x) {}
		inline bigint(const std::string &str) {
			/* TODO: Implement the constructor from string */
		}
		/* TODO: Implement copy and move constructors */

		inline bigint &operator<<=(size_t n) {
			if (m_data.empty()) {
				return *this;
			}
			size_t m = (n >> log_bits); // m = n / unit_bits
			n &= unit_bits - 1;			// n %= unit_bits
			if (!n) {
				/*
				 * A right shift by 64 bits leads to undefined behavior.
				 * So we need to handle separately the case that n == 0
				 * to avoid this problem.
				 */
				m_data.insert(m_data.begin(), m, 0);
				return *this;
			}
			size_t sz = m_data.size();
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
		inline friend bigint operator<<(bigint lhs, size_t n) {
			return (lhs <<= n);
		}

		inline bigint &operator>>=(size_t n) {
			if (m_data.empty()) {
				return *this;
			}
			size_t m = (n >> log_bits); // m = n / unit_bits
			if (m >= m_data.size()) {
				m_data.clear();
				return *this;
			}
			n &= unit_bits - 1; // n %= unit_bits
			if (!n) {
				m_data.erase(m_data.begin(), m_data.begin() + m);
				return *this;
			}
			for (size_t i = 0; i + m + 1 != m_data.size(); ++i) {
				m_data[i] = ((m_data[i + m] >> n) |
							 (m_data[i + m + 1] << (unit_bits - n)));
			}
			if (m_data.back() >> n) {
				m_data[m_data.size() - m - 1] = (m_data.back() >> n);
			} else {
				++m;
			}
			m_data.resize(m_data.size() - m);
			return *this;
		}
		inline friend bigint operator>>(bigint lhs, size_t n) {
			return (lhs >>= n);
		}

		inline friend bigint operator~(bigint x) {
			size_t i = x.m_data.size() - 1;
			for (; ~i && x.m_data[i] == static_cast<uint64_t>(-1); --i) {
				x.m_data.pop_back();
			}
			for (; ~i; --i) {
				x.m_data[i] = ~x.m_data[i];
			}
			return x;
		}

		inline bigint &operator&=(const bigint &rhs) {
			size_t i = std::min(m_data.size(), rhs.m_data.size()) - 1;
			while (~i && !(m_data[i] & rhs.m_data[i])) {
				--i;
			}
			m_data.resize(i + 1);
			for (; ~i; --i) {
				m_data[i] &= rhs.m_data[i];
			}
			return *this;
		}
		inline friend bigint operator&(const bigint &lhs, const bigint &rhs) {
			size_t i = std::min(lhs.m_data.size(), rhs.m_data.size()) - 1;
			while (~i && !(lhs.m_data[i] & rhs.m_data[i])) {
				--i;
			}
			bigint res;
			res.m_data.resize(i + 1);
			for (; ~i; --i) {
				res.m_data[i] = (lhs.m_data[i] & rhs.m_data[i]);
			}
			return res;
		}

		inline bigint &operator|=(const bigint &rhs) {
			size_t i = 0;
			for (; i != m_data.size() && i != rhs.m_data.size(); ++i) {
				m_data[i] |= rhs.m_data[i];
			}
			if (i != rhs.m_data.size()) {
				m_data.insert(m_data.end(), rhs.m_data.begin() + i, rhs.m_data.end());
			}
			return *this;
		}
		inline friend bigint operator|(const bigint &lhs, const bigint &rhs) {
			bigint res;
			res.m_data.resize(std::max(lhs.m_data.size(), rhs.m_data.size()));
			size_t i = 0;
			for (; i != lhs.m_data.size() && i != rhs.m_data.size(); ++i) {
				res.m_data[i] = (lhs.m_data[i] | rhs.m_data[i]);
			}
			for (; i != lhs.m_data.size(); ++i) {
				res.m_data[i] = lhs.m_data[i];
			}
			for (; i < rhs.m_data.size(); ++i) {
				res.m_data[i] = rhs.m_data[i];
			}
			return res;
		}

		inline bool operator==(const bigint &other) const {
			return m_data == other.m_data;
		}

		inline friend std::ostream &testOut(std::ostream &os, const bigint &x) {
			if (x.m_data.empty()) {
				return os << 0;
			}
			os << x.m_data.back();
			for (size_t i = x.m_data.size() - 2; ~i; --i) {
				os << ' ' << x.m_data[i];
			}
			return os;
		}

	protected:
		std::vector<uint64_t> m_data;

		inline void m_expand(size_t n) { // Suppose n >= m_data.size()
			if (m_data.empty()) {
				m_data.resize(n);
			} else if (m_data.back() & (static_cast<uint64_t>(1) << (unit_bits - 1))) {
				m_data.resize(n, static_cast<uint64_t>(-1));
			} else {
				m_data.resize(n);
			}
		}
	}; // class bigint

} // namespace ds

#endif // _BIGUINT_HPP
