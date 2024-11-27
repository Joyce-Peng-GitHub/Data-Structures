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
				 bool _nega = false)
			: abs(_num, _den), __nega(_nega && _num) {}
		rational(const ur_t &_u, bool _nega = false)
			: abs(_u), __nega(_nega && _u.__num) {}

		inline r_t &assign(const uint_t &_num = 0, const uint_t &_den = 1,
						   bool _nega = false) {
			this->abs.assign(_num, _den);
			this->__nega = (_nega && _num);
			return *this;
		}
		inline r_t &assign(const ur_t &_ur, bool _nega) {
			this->abs.assign(_ur);
			this->__nega = (_nega && _ur.__num);
			return *this;
		}
		inline r_t &assign(const r_t &_r) {
			this->abs.assign(_r.abs);
			this->__nega = _r.__nega;
			return *this;
		}

		inline static r_t zero() { return {}; }
		inline static r_t one() { return 1; }
		inline static r_t posiinf() { return {1, 0}; }
		inline static r_t negainf() { return {1, 0, true}; }
		inline static r_t nan() { return {0, 0}; }

		inline uint_t numerator() const { return this->abs.__num; }
		inline uint_t denominator() const { return this->abs.__den; }

		inline bool iszero() const { return this->abs.iszero(); }
		inline bool isone() const {
			return (!this->__nega && this->abs.isone());
		}
		inline bool ispositive() const {
			return (!this->__nega && this->abs.__num);
		}
		inline bool isnegative() const {
			return (this->__nega && this->abs.__num);
		}
		inline bool isfinite() const { return this->abs.isfinite(); }
		inline bool isinf() const { return this->abs.isinf(); }
		inline bool isnan() const { return this->abs.isnan(); }
		inline bool notnan() const { return this->abs.notnan(); }
		inline bool isposiinf() const {
			return (!this->__nega && this->abs.isinf());
		}
		inline bool isnegainf() const {
			return (this->__nega && this->abs.isinf());
		}
		inline int sign() const {
			return (this->__nega ? -1 : (this->abs.__num ? 1 : 0));
		}
		inline r_t negate() const {
			return {this->abs, !this->__nega};
		}
		inline r_t &tonegative() {
			this->__nega = !this->__nega;
			return *this;
		}
		inline r_t reciprocol() const {
			return {this->abs.__den, this->abs.__num, this->__nega};
		}
		inline r_t &toreciprocol() {
			this->abs.toreciprocol();
			return *this;
		}
		inline r_t operator-() const { return this->negate(); }

		inline operator long double() const {
			return (this->__nega
						? -static_cast<long double>(this->abs)
						: static_cast<long double>(this->abs));
		}
		inline operator std::string() const {
			return ((this->__nega && this->isfinite())
						? ('-' + static_cast<std::string>(this->abs))
						: static_cast<std::string>(this->abs));
		}

		inline static bool equal(const r_t &_lhs, const r_t &_rhs) {
			return (_lhs.__nega == _rhs.__nega &&
					_lhs.abs == _rhs.abs);
		}
		inline static bool not_equal(const r_t &_lhs, const r_t &_rhs) {
			return !equal(_lhs, _rhs);
		}
		inline static bool less(const r_t &_lhs, const r_t &_rhs) {
			return ((_lhs.__nega == _rhs.__nega)
						? (_lhs.__nega
							   ? (_lhs.abs > _rhs.abs)
							   : (_lhs.abs < _rhs.abs))
						: (_lhs.__nega > _rhs.__nega));
		}
		inline static bool less_equal(const r_t &_lhs, const r_t &_rhs) {
			return ((_lhs.__nega == _rhs.__nega)
						? (_lhs.__nega
							   ? (_lhs.abs >= _rhs.abs)
							   : (_lhs.abs <= _rhs.abs))
						: (_lhs.__nega > _rhs.__nega));
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
			if (this->__nega == _rhs.__nega) {
				this->abs += _rhs.abs;
			} else {
				if (this->abs >= _rhs.abs) {
					this->abs.__minus_equal(_rhs.abs);
				} else {
					ur_t rhsabs = _rhs.abs;
					this->__nega = _rhs.__nega;
					this->abs = rhsabs.__minus_equal(this->abs);
				}
			}
			this->__check_and_negate();
			return *this;
		}
		inline static r_t plus(r_t _lhs, const r_t &_rhs) {
			return _lhs.plus_equal(_rhs);
		}
		inline r_t &minus_equal(r_t _rhs) {
			return this->plus_equal(_rhs.tonegative());
		}
		inline static r_t minus(r_t _lhs, r_t _rhs) {
			return _lhs.plus_equal(_rhs.tonegative());
		}
		inline r_t &multiplies_equal(const r_t &_rhs) {
			this->__nega ^= this->__nega;
			this->abs *= _rhs.abs;
			this->__check_and_negate();
			return *this;
		}
		inline static r_t multiplies(r_t _lhs, const r_t &_rhs) {
			return _lhs.multiplies_equal(_rhs);
		}
		inline r_t &divides_equal(const r_t &_rhs) {
			return this->multiplies_equal(_rhs.reciprocol());
		}
		inline static r_t divides(r_t _lhs, const r_t &_rhs) {
			return _lhs.divides_equal(_rhs);
		}
		inline r_t &operator+=(const r_t &_rhs) {
			return this->plus_equal(_rhs);
		}
		inline friend r_t operator+(const r_t &_lhs, const r_t &_rhs) {
			return plus(_lhs, _rhs);
		}
		inline r_t &operator-=(const r_t &_rhs) {
			return this->minus_equal(_rhs);
		}
		inline friend r_t operator-(const r_t &_lhs, const r_t &_rhs) {
			return minus(_lhs, _rhs);
		}
		inline r_t &operator*=(const r_t &_rhs) {
			return this->multiplies_equal(_rhs);
		}
		inline friend r_t operator*(const r_t &_lhs, const r_t &_rhs) {
			return multiplies(_lhs, _rhs);
		}
		inline r_t &operator/=(const r_t &_rhs) {
			return this->divides_equal(_rhs);
		}
		inline friend r_t operator/(const r_t &_lhs, const r_t &_rhs) {
			return divides(_lhs, _rhs);
		}
		inline friend std::istream &operator>>(std::istream &_is, r_t &_r) {
			char sign;
			_is >> sign;
			_r.__nega = (sign == '-');
			if (sign != '-' && sign != '+') {
				_is.putback(sign);
			}
			if (_is >> _r.abs) {
				_r.__check_and_negate();
			} else {
				_r.assign();
			}
			return _is;
		}
		inline friend std::ostream &operator<<(std::ostream &_os,
											   const r_t &_r) {
			return ((_r.isfinite() && _r.__nega) ? (_os << '-') : _os)
				   << _r.abs;
		}

	protected:
		bool __nega;

		inline void __check_and_negate() {
			if (this->__nega && !this->abs.__num) {
				this->__nega = false;
			}
		}

	private:
	};
}

#endif