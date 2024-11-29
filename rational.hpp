#ifndef _RATIONAL_HPP
#define _RATIONAL_HPP

#include "unsigned_rational.hpp"

namespace ds {

	template <typename uint_t = uint64_t>
	class rational {
	public:
		using ur_t = unsigned_rational<uint_t>;
		using r_t = rational<uint_t>;

		ur_t abs;

		rational(const uint_t &_num = 0, const uint_t &_den = 1,
				 bool _neg = false)
			: abs(_num, _den), __neg(_neg && _num) {}
		rational(const ur_t &_u, bool _neg = false)
			: abs(_u), __neg(_neg && _u.__num) {}

		inline r_t &assign(const uint_t &_num = 0, const uint_t &_den = 1,
						   bool _neg = false) {
			abs.assign(_num, _den);
			__neg = (_neg && _num);
			return *this;
		}
		inline r_t &assign(const ur_t &_ur, bool _neg) {
			abs.assign(_ur);
			__neg = (_neg && _ur.__num);
			return *this;
		}
		inline r_t &assign(const r_t &_r) {
			abs.assign(_r.abs);
			__neg = _r.__neg;
			return *this;
		}

		inline static r_t zero() { return {}; }
		inline static r_t one() { return 1; }
		inline static r_t posiinf() { return {1, 0}; }
		inline static r_t negainf() { return {1, 0, true}; }
		inline static r_t nan() { return {0, 0}; }

		inline uint_t numerator() const { return abs.__num; }
		inline uint_t denominator() const { return abs.__den; }

		inline bool iszero() const { return abs.iszero(); }
		inline bool isone() const {
			return (!__neg && abs.isone());
		}
		inline bool ispositive() const {
			return (!__neg && abs.__num);
		}
		inline bool isnegative() const {
			return (__neg && abs.__num);
		}
		inline bool isfinite() const { return abs.isfinite(); }
		inline bool isinf() const { return abs.isinf(); }
		inline bool isnan() const { return abs.isnan(); }
		inline bool notnan() const { return abs.notnan(); }
		inline bool isposinf() const {
			return (!__neg && abs.isinf());
		}
		inline bool isneginf() const {
			return (__neg && abs.isinf());
		}
		inline int sign() const {
			return (__neg ? -1 : (abs.__num ? 1 : 0));
		}
		inline r_t opposite() const {
			return {abs, !__neg};
		}
		inline r_t &toopposite() {
			__neg = !__neg;
			return *this;
		}
		inline r_t reciprocal() const {
			return {abs.__den, abs.__num, __neg};
		}
		inline r_t &toreciprocal() {
			abs.toreciprocal();
			return *this;
		}
		inline r_t operator-() const { return opposite(); }

		inline operator long double() const {
			return (__neg
						? -static_cast<long double>(abs)
						: static_cast<long double>(abs));
		}
		inline operator std::string() const {
			return ((__neg && isfinite())
						? ('-' + static_cast<std::string>(abs))
						: static_cast<std::string>(abs));
		}

		inline static bool equal(const r_t &_lhs, const r_t &_rhs) {
			return (_lhs.__neg == _rhs.__neg &&
					_lhs.abs == _rhs.abs);
		}
		inline static bool not_equal(const r_t &_lhs, const r_t &_rhs) {
			return !equal(_lhs, _rhs);
		}
		inline static bool less(const r_t &_lhs, const r_t &_rhs) {
			return ((_lhs.__neg == _rhs.__neg)
						? (_lhs.__neg
							   ? (_lhs.abs > _rhs.abs)
							   : (_lhs.abs < _rhs.abs))
						: (_lhs.__neg > _rhs.__neg));
		}
		inline static bool less_equal(const r_t &_lhs, const r_t &_rhs) {
			return ((_lhs.__neg == _rhs.__neg)
						? (_lhs.__neg
							   ? (_lhs.abs >= _rhs.abs)
							   : (_lhs.abs <= _rhs.abs))
						: (_lhs.__neg > _rhs.__neg));
		}
		inline static bool greater(const r_t &_lhs, const r_t &_rhs) {
			return less(_rhs, _lhs);
		}
		inline static bool greater_equal(const r_t &_lhs, const r_t &_rhs) {
			return less_equal(_rhs, _lhs);
		}
		inline friend bool operator==(const r_t &_lhs, const r_t &_rhs) {
			return equal(_lhs, _rhs);
		}
		inline friend bool operator!=(const r_t &_lhs, const r_t &_rhs) {
			return not_equal(_lhs, _rhs);
		}
		inline friend bool operator<(const r_t &_lhs, const r_t &_rhs) {
			return less(_lhs, _rhs);
		}
		inline friend bool operator<=(const r_t &_lhs, const r_t &_rhs) {
			return less_equal(_lhs, _rhs);
		}
		inline friend bool operator>(const r_t &_lhs, const r_t &_rhs) {
			return greater(_lhs, _rhs);
		}
		inline friend bool operator>=(const r_t &_lhs, const r_t &_rhs) {
			return greater_equal(_lhs, _rhs);
		}

		inline r_t &plus_equal(const r_t &_rhs) {
			if (__neg == _rhs.__neg) {
				abs += _rhs.abs;
			} else {
				if (abs >= _rhs.abs) {
					abs.__minus_equal(_rhs.abs);
				} else {
					ur_t rhsabs = _rhs.abs;
					__neg = _rhs.__neg;
					abs = rhsabs.__minus_equal(abs);
				}
			}
			__check_and_set_sign();
			return *this;
		}
		inline static r_t plus(r_t _lhs, const r_t &_rhs) {
			return _lhs.plus_equal(_rhs);
		}
		inline r_t &minus_equal(r_t _rhs) {
			return plus_equal(_rhs.toopposite());
		}
		inline static r_t minus(r_t _lhs, r_t _rhs) {
			return _lhs.plus_equal(_rhs.toopposite());
		}
		inline r_t &multiplies_equal(const r_t &_rhs) {
			__neg ^= _rhs.__neg;
			abs *= _rhs.abs;
			__check_and_set_sign();
			return *this;
		}
		inline static r_t multiplies(r_t _lhs, const r_t &_rhs) {
			return _lhs.multiplies_equal(_rhs);
		}
		inline r_t &divides_equal(const r_t &_rhs) {
			return multiplies_equal(_rhs.reciprocal());
		}
		inline static r_t divides(r_t _lhs, const r_t &_rhs) {
			return _lhs.divides_equal(_rhs);
		}
		inline r_t &operator+=(const r_t &_rhs) {
			return plus_equal(_rhs);
		}
		inline friend r_t operator+(const r_t &_lhs, const r_t &_rhs) {
			return plus(_lhs, _rhs);
		}
		inline r_t &operator-=(const r_t &_rhs) {
			return minus_equal(_rhs);
		}
		inline friend r_t operator-(const r_t &_lhs, const r_t &_rhs) {
			return minus(_lhs, _rhs);
		}
		inline r_t &operator*=(const r_t &_rhs) {
			return multiplies_equal(_rhs);
		}
		inline friend r_t operator*(const r_t &_lhs, const r_t &_rhs) {
			return multiplies(_lhs, _rhs);
		}
		inline r_t &operator/=(const r_t &_rhs) {
			return divides_equal(_rhs);
		}
		inline friend r_t operator/(const r_t &_lhs, const r_t &_rhs) {
			return divides(_lhs, _rhs);
		}
		inline friend std::istream &operator>>(std::istream &_is, r_t &_r) {
			char sign;
			_is >> sign;
			_r.__neg = (sign == '-');
			if (sign != '-' && sign != '+') {
				_is.putback(sign);
			}
			if (_is >> _r.abs) {
				_r.__check_and_set_sign();
			} else {
				_r.assign();
			}
			return _is;
		}
		inline friend std::ostream &operator<<(std::ostream &_os,
											   const r_t &_r) {
			return ((_r.isfinite() && _r.__neg) ? (_os << '-') : _os)
				   << _r.abs;
		}

	protected:
		bool __neg;

		inline void __check_and_set_sign() {
			if (__neg && !abs.__num) {
				__neg = false;
			}
		}

	private:
	};
}

#endif