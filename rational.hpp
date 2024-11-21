#ifndef _RATIONAL_HPP
#define _RATIONAL_HPP

#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>

namespace ds {
	template <typename uint_t = uint64_t>
	class unsigned_rational {
	public:
		using ur_t = unsigned_rational<uint_t>;

		inline static uint_t gcd(uint_t _x, uint_t _y) {
			uint_t temp = _y;
			if (_x < _y) {
				_y = _x;
				_x = temp;
			}
			while (_y) {
				temp = _y;
				_y = _x % _y;
				_x = temp;
			}
			return _x;
		}
		inline static uint_t lcm(uint_t _x, uint_t _y) {
			return (_x / gcd(_x, _y));
		}

		inline uint_t numerator() const { return this->__num; }
		inline uint_t denominator() const { return this->__den; }

		inline bool iszero() const { return (!this->__num && this->__den); }
		inline bool isone() const { return (this->__num == this->__den); }
		inline bool isfinite() const { return this->__den; }
		inline bool isinf() const { return (this->num && !this->__den); }
		inline bool isnan() const { return (!this->__num && !this->__den); }

		unsigned_rational(uint_t _num = 0, uint_t _den = 1)
			: __num(_num), __den(_den) { this->__normalize(); }

		inline ur_t reciprocol() const {
			return (this->isfinite()
						? ur_t(this->__den, this->__num)
						: (this->__num ? zero() : nan()));
		}
		inline ur_t inverse() const { return this->reciprocol(); }

		inline static ur_t zero() { return {0, 1}; }
		inline static ur_t one() { return {1, 1}; }
		inline static ur_t inf() { return {1, 0}; }
		inline static ur_t nan() { return {0, 0}; }

		inline static bool equal(const ur_t &_lhs, const ur_t &_rhs) {
			return ((_lhs.__num == _rhs.__num) && (_lhs.__den == _rhs.__den));
		}
		inline static bool not_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return !equal(_lhs, _rhs);
		}
		inline static bool less(const ur_t &_lhs, const ur_t &_rhs) {
			return (static_cast<long double>(_lhs) <
					static_cast<long double>(_rhs));
		}
		inline static bool less_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return (static_cast<long double>(_lhs) <=
					static_cast<long double>(_rhs));
		}
		inline static bool greater(const ur_t &_lhs, const ur_t &_rhs) {
			return (static_cast<long double>(_lhs) >
					static_cast<long double>(_rhs));
		}
		inline static bool greater_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return (static_cast<long double>(_lhs) >=
					static_cast<long double>(_rhs));
		}
		inline friend bool operator==(const ur_t &_lhs, const ur_t &_rhs) {
			return equal(_lhs, _rhs);
		}
		inline friend bool operator!=(const ur_t &_lhs, const ur_t &_rhs) {
			return not_equal(_lhs, _rhs);
		}
		inline friend bool operator<(const ur_t &_lhs, const ur_t &_rhs) {
			return less(_lhs, _rhs);
		}
		inline friend bool operator<=(const ur_t &_lhs, const ur_t &_rhs) {
			return less_equal(_lhs, _rhs);
		}
		inline friend bool operator>(const ur_t &_lhs, const ur_t &_rhs) {
			return greater(_lhs, _rhs);
		}
		inline friend bool operator>=(const ur_t &_lhs, const ur_t &_rhs) {
			return greater_equal(_lhs, _rhs);
		}

		inline operator uint_t() const { return (__num / __den); }
		inline operator long double() const {
			return (this->__num / static_cast<long double>(this->__num));
		}
		inline operator std::string() const {
			return (this->__den
						? (std::to_string(this->__num) + '/' +
						   std::to_string(this->__den))
						: (this->__num ? "inf" : "nan"));
		}

		/**
		 * f1 + f2 == f3
		 * inf + f == inf
		 * nan + f == nan
		 * inf + f == inf
		 * inf + inf == inf
		 * nan + inf == nan
		 * nan + nan == nan
		 */
		inline ur_t &plus_equal(const ur_t &_rhs) {
			if (this->isfinite()) {
				if (_rhs.isfinite()) {
					uint_t d = gcd(this->__den, _rhs.__den);
					this->__num = _rhs.__den / d * this->__num +
								  this->__den / d * _rhs.__num;
					this->__den *= _rhs.__den / d;
				} else if (_rhs.__num) { // _rhs == inf
					*this = inf();
				} else { // _rhs == nan
					*this = nan();
				}
			} else if (this->__num) { // *this == inf
				if (_rhs.isnan()) {
					*this = nan();
				}
				/* Otherwise let *this stay still */
			}
			return *this;
		}
		inline static ur_t plus(ur_t _lhs, const ur_t &_rhs) {
			return _lhs.plus_equal(_rhs);
		}
		inline ur_t &minus_equal(const ur_t &_rhs) {
			if (*this >= _rhs) {
				return __minus_equal(_rhs);
			}
			if (!_rhs.isnan()) {
				std::__throw_invalid_argument(
					("unsigned_rational::minus_equal: _rhs (which is " +
					 std::string(_rhs) + ") > *this (which is " +
					 std::string(*this) + ')')
						.c_str());
			}
			std::__throw_invalid_argument(
				"unsigned_rational::minus_equal: _rhs is nan");
		}
		inline static ur_t minus(ur_t _lhs, const ur_t &_rhs) {
			return _lhs.minus_equal(_rhs);
		}
		/**
		 * f1 * f2 == f3
		 * inf * 0 == nan
		 * inf * f == inf
		 * inf * inf == inf
		 * nan * f == nan
		 * nan * inf == nan
		 * nan * nan == nan
		 */
		inline ur_t &multiplies_equal(const ur_t &_rhs) {
			if (this->isfinite()) {
				if (_rhs.isfinite()) {
					this->__num =
						this->__num / gcd(this->__num, _rhs.__den) *
						_rhs.__num;
					this->__den =
						this->__den / gcd(this->__den, _rhs.__num) *
						_rhs.__den;
				} else if (_rhs.__num) {			 // _rhs == inf
					this->__den = 0;				 // set *this to inf
				} else {							 // _rhs == nan
					this->__num = (this->__den = 0); // set *this to nan
				}
			} else if (this->__num) { // *this == inf
				if (_rhs.isnan()) {
					this->__num = 0;
				}
			}
			return *this;
		}
		inline static ur_t multiplies(ur_t _lhs, const ur_t &_rhs) {
			return _lhs.multiplies_equal(_rhs);
		}
		inline ur_t &divides_equal(const ur_t &_rhs) {
			return this->multiplies_equal(_rhs.reciprocol());
		}
		inline static ur_t divides(ur_t _lhs, const ur_t &_rhs) {
			return _lhs.divides_equal(_rhs);
		}
		inline ur_t &operator+=(const ur_t &_rhs) {
			return this->plus_equal(_rhs);
		}
		inline friend ur_t operator+(const ur_t &_lhs, const ur_t &_rhs) {
			return plus(_lhs, _rhs);
		}
		inline ur_t &operator-=(const ur_t &_rhs) {
			return this->minus_equal(_rhs);
		}
		inline friend ur_t operator-(const ur_t &_lhs, const ur_t &_rhs) {
			return minus(_lhs, _rhs);
		}
		inline ur_t &operator*=(const ur_t &_rhs) {
			return this->multiplies_equal(_rhs);
		}
		inline friend ur_t operator*(const ur_t &_lhs, const ur_t &_rhs) {
			return multiplies(_lhs, _rhs);
		}
		inline ur_t &operator/=(const ur_t &_rhs) {
			return this->divides_equal(_rhs);
		}
		inline friend ur_t operator/(const ur_t &_lhs, const ur_t &_rhs) {
			return divides(_lhs, _rhs);
		}

		inline friend std::istream &operator>>(std::istream &is, ur_t &_rhs) {
			char ch;
			is >> _rhs.__num;
			if (!is.good()) {
				_rhs.__num = 0;
				_rhs.__den = 1;
				goto RET;
			}
			is.get(ch);
			if (is.fail() || ch != '/') {
				std::cin.clear();
				std::cin.putback(ch);
				_rhs.__den = 1;
				goto RET;
			}
			if (is >> _rhs.__den) {
				_rhs.__normalize();
			} else {
				_rhs.__num = 0;
				_rhs.__den = 1;
			}
		RET:
			return is;
		}
		inline friend std::ostream &operator<<(std::ostream &os,
											   const ur_t &_rhs) {
			return (_rhs.isfinite()
						? ((_rhs.__den == 1)
							   ? (os << _rhs.__num)
							   : (os << _rhs.__num << '/' << _rhs.__den))
						: (_rhs.__num ? (os << INFINITY) : (os << NAN)));
		}

	protected:
		uint_t __num, __den;

		inline void __normalize() {
			if (this->__num && this->__den) {
				uint_t d = gcd(this->__num, this->__den);
				this->__num /= d;
				this->__den /= d;
			}
		}
		/**
		 * f1 - f2 == f3
		 * f - inf == -inf
		 * inf - f == inf
		 * f - nan == nan
		 * nan - f == nan
		 * inf - inf == inf
		 * nan - inf == nan
		 * inf - nan == nan
		 * nan - nan == nan
		 *
		 * __minus_equal assumes that *this >= _rhs.,
		 * so *this must not be nan.
		 */
		inline ur_t &__minus_equal(const ur_t &_rhs) {
			if (this->isfinite()) {

				uint_t d = gcd(this->__den, _rhs.__den);
				this->__num = _rhs.__den / d * this->__num -
							  this->__den / d * _rhs.__num;
				this->__den *= _rhs.__den / d;
			} else if (this->__num) { // *this == inf
				if (!_rhs.__den) {	  // _rhs == inf
					*this = nan();	  // inf - inf == nan
				}
			}
			return *this;
		}

	private:
	};
}

#endif