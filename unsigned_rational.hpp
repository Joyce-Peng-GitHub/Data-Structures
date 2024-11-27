#ifndef _UNSIGNED_RATIONAL_HPP
#define _UNSIGNED_RATIONAL_HPP

#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>

namespace ds {
	template <typename uint_t>
	class rational;

	template <typename uint_t = uint64_t>
	class unsigned_rational {
	public:
		friend rational<uint_t>;
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

		unsigned_rational(uint_t _num = 0, uint_t _den = 1)
			: __num(_num), __den(_den) { this->__normalize(); }

		inline ur_t &assign(uint_t _num = 0, uint_t _den = 1) {
			this->__assign(_num, _den);
			return this->__normalize();
		}
		inline ur_t &assign(const ur_t &_ur) {
			return this->__assign(_ur.__num, _ur.__den);
		}

		inline static ur_t zero() { return {}; }
		inline static ur_t one() { return 1; }
		inline static ur_t inf() { return {1, 0}; }
		inline static ur_t nan() { return {0, 0}; }

		inline uint_t numerator() const { return this->__num; }
		inline uint_t denominator() const { return this->__den; }

		inline bool iszero() const { return (!this->__num && this->__den); }
		inline bool isone() const { return (this->__num == this->__den); }
		inline bool isfinite() const { return this->__den; }
		inline bool isinf() const { return (this->num && !this->__den); }
		inline bool isnan() const { return (!this->__num && !this->__den); }
		inline bool notnan() const { return (this->__num && this->__den); }

		inline ur_t reciprocol() const {
			return (this->isfinite()
						? ur_t(this->__den, this->__num)
						: (this->__num ? zero() : nan()));
		}
		inline ur_t &toreciprocol() {
			std::swap(this->__den, this->__num);
			return *this;
		}
		inline ur_t inverse() const { return this->reciprocol(); }

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

		// nan is not comparable
		inline static bool equal(const ur_t &_lhs, const ur_t &_rhs) {
			return ((_lhs.__num == _rhs.__num) && (_lhs.__den == _rhs.__den) &&
					(_lhs.notnan()));
		}
		inline static bool not_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return !equal(_lhs, _rhs);
		}
		inline static bool less(const ur_t &_lhs, const ur_t &_rhs) {
			return (_lhs.__num * _rhs.__den < _lhs.__den * _rhs.__num);
		}
		inline static bool less_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return ((_lhs.__num * _rhs.__den <= _lhs.__den * _rhs.__num) &&
					_lhs.notnan());
		}
		inline static bool greater(const ur_t &_lhs, const ur_t &_rhs) {
			return (_lhs.__num * _rhs.__den > _lhs.__den * _rhs.__num);
		}
		inline static bool greater_equal(const ur_t &_lhs, const ur_t &_rhs) {
			return ((_lhs.__num * _rhs.__den >= _lhs.__den * _rhs.__num) &&
					_lhs.notnan());
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
					this->__plus_equal(_rhs);
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
				return this->__minus_equal(_rhs);
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
			return *this;
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

		inline friend std::istream &operator>>(std::istream &_is, ur_t &_ur) {
			char ch;
			uint_t n = 0, d = 1;
			if (!(_is >> n)) {
				goto RET;
			}
			_is >> ch;
			if (_is.fail() || ch != '/') {
				_is.clear();
				_is.putback(ch);
				goto RET;
			}
			_is >> d;
		RET:
			_ur.assign(n, d);
			return _is;
		}
		inline friend std::ostream &operator<<(std::ostream &_os,
											   const ur_t &_ur) {
			return (_ur.isfinite()
						? ((_ur.__den == 1)
							   ? (_os << _ur.__num)
							   : (_os << _ur.__num << '/' << _ur.__den))
						: (_ur.__num ? (_os << INFINITY) : (_os << NAN)));
		}

	protected:
		uint_t __num, __den;

		inline ur_t &__assign(const uint_t &_num, const uint_t &_den) {
			this->__num = _num;
			this->__den = _den;
			return *this;
		}
		inline ur_t &__normalize() {
			if (this->__num) {
				if (this->__den) {
					uint_t d = gcd(this->__num, this->__den);
					this->__num /= d;
					this->__den /= d;
				} else {
					this->__num = 1;
				}
			} else {
				if (this->__den) {
					this->__den = 1;
				}
			}
			return *this;
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
		inline ur_t &__plus_equal(const ur_t &_rhs) {
			uint_t d = gcd(this->__den, _rhs.__den);
			this->__num = _rhs.__den / d * this->__num +
						  this->__den / d * _rhs.__num;
			this->__den *= _rhs.__den / d;
			this->__normalize();
			return *this;
		}
		inline ur_t &__minus_equal(const ur_t &_rhs) {
			if (this->isfinite()) {

				uint_t d = gcd(this->__den, _rhs.__den);
				this->__num = _rhs.__den / d * this->__num -
							  this->__den / d * _rhs.__num;
				this->__den *= _rhs.__den / d;
				this->__normalize();
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