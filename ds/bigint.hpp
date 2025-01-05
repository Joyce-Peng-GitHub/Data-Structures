#ifndef _BIGUINT_HPP
#define _BIGUINT_HPP

#include <cstdint>
#include <vector>

namespace ds {
	class bigint {
	public:
		using uint64_t = std::uint64_t;
		using size_t = std::size_t;

		static constexpr size_t BITS = 64, LOG_BITS = 6;

		inline bigint() = default;
		inline bigint(int64_t x) : m_data(1, x) {}

		inline bigint &operator<<=(size_t n) {
			if (m_data.empty()) {
				return *this;
			}
			size_t m = (n >> LOG_BITS); // m = n / BITS
			n &= BITS - 1;				// n %= BITS
			/**
			 * A right shift by 64 bits leads to undefined behavior.
			 * So we need to handle seperately the case that n == 0
			 * to avoid this problem.
			 */
			if (!n) {
				m_data.insert(m_data.begin(), m, 0);
				return *this;
			}
			size_t sz = m_data.size();
			m_data.resize(sz + m);
			if (m_data[sz - 1] >> (BITS - n)) {
				m_data.push_back(m_data[sz - 1] >> (BITS - n));
			}
			for (size_t i = sz + m - 1; i != m; --i) {
				m_data[i] = ((m_data[i - m] << n) | (m_data[i - m - 1] >> (BITS - n)));
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

	protected:
		std::vector<uint64_t> m_data;
	};
}

#endif