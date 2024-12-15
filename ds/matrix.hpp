#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <cstddef>
#include <utility>
#include <vector>

#include "group.hpp"

namespace ds {
	template <typename elem_t,
			  elem_t id_elem = 1,
			  typename plus_t = std::plus<elem_t>,
			  typename multiplies_t = std::multiplies<elem_t>>
	class matrix {
	public:
		using vector = std::vector<elem_t>;
		struct __vector_base {
			virtual elem_t &at(size_t _n) = 0;
		};
		struct row_vector {
			size_t row;
			matrix &mat;

			inline elem_t &at(size_t _n) const {
				return mat.__mat[row].at(_n);
			}
			inline elem_t &operator[](size_t _n) const {
				return mat.__mat[row][_n];
			}
			inline operator vector() const { return mat.__mat[row]; }
		};
		struct const_row_vector {
			size_t row;
			const matrix &mat;

			inline const elem_t &at(size_t _n) const {
				return mat.__mat[row].at(_n);
			}
			inline const elem_t &operator[](size_t _n) const {
				return mat.__mat[row][_n];
			}

			inline operator vector() const {
				return mat.__mat[row];
			}
		};
		struct col_vector {
			size_t col;
			matrix &mat;

			inline elem_t &at(size_t _n) const {
				return mat.__mat.at(_n)[col];
			}
			inline elem_t &operator[](size_t _n) const {
				return mat.__mat[_n][col];
			}
			inline operator vector() const {
				vector res(mat.__n);
				for (size_t i = 0; i != mat.__n; ++i) {
					res[i] = mat.__mat[i][col];
				}
				return res;
			}
		};
		struct const_col_vector {
			size_t col;
			const matrix &mat;

			inline const elem_t &at(size_t _n) const {
				return mat.__mat.at(_n)[col];
			}
			inline const elem_t &operator[](size_t _n) const {
				return mat.__mat[_n][col];
			}
			inline operator vector() const {
				vector res(mat.__n);
				for (size_t i = 0; i != mat.__n; ++i) {
					res[i] = mat.__mat[i][col];
				}
				return res;
			}
		};
		struct sub_matrix {
			size_t row, col, n, m;
			matrix &mat;

			inline operator matrix() const {
				if (!n || !m) {
					return matrix();
				}
				matrix res(n, m);
				for (size_t i = 0, j; i != n; ++i) {
					for (j = 0; j != m; ++j) {
						res.__mat[i][j] = mat.__mat[row + i][col + j];
					}
				}
				return res;
			}
		};

		matrix() = default;
		matrix(size_t _n, size_t _m)
			: __mat((_m ? _n : 0), vector((_n ? _m : 0))) {}
		matrix(std::initializer_list<std::initializer_list<elem_t>> _mat)
			: __mat(_mat.size()) {
			size_t m = 0, i = 0;
			for (auto &r : _mat) {
				__mat[i++] = r;
				if (r.size() > m) {
					m = r.size();
				}
			}
			for (auto &r : __mat) {
				r.resize(m);
			}
			if (!m) {
				__mat.clear();
			}
		}
		matrix(const std::vector<vector> &_mat)
			: __mat(_mat) {
			size_t m = 0;
			for (const auto &r : _mat) {
				if (r.size() > m) {
					m = r.size();
				}
			}
			if (m) {
				for (auto &r : __mat) {
					r.resize(m);
				}
			} else {
				__mat.clear();
			}
		}

		inline static matrix null() { return matrix(); }
		inline static matrix zero(size_t _n, size_t _m) {
			return matrix(_n, _m);
		}
		inline static matrix identity(size_t _n) {
			matrix res(_n, _n);
			for (size_t i = 0; i != _n; ++i) {
				res.__mat[i][i] = id_elem;
			}
			return res;
		}
		inline static matrix diagnal(const vector &_diag) {
			matrix res(_diag.size(), _diag.size());
			for (size_t i = 0; i != _diag.size(); ++i) {
				res.__mat[i][i] = _diag[i];
			}
			return res;
		}

		inline size_t row() const { return __mat.size(); }
		inline size_t col() const {
			return (__mat.size() ? __col() : 0);
		}
		inline std::pair<size_t, size_t> size() const {
			return {row(), col()};
		}

		inline const std::vector<vector> &data() const {
			return __mat;
		}
		inline row_vector row(size_t _n) { return {_n, *this}; }
		inline const_row_vector row(size_t _n) const { return {_n, *this}; }
		inline row_vector row_at(size_t _n) {
			if (_n >= row()) {
				std::__throw_out_of_range_fmt(
					__N("matrix::row_at: _n "
						"(which is %zu) >= this->row() "
						"(which is %zu)"),
					_n, __mat.size());
			}
			return row(_n);
		}
		inline const_row_vector row_at(size_t _n) const {
			if (_n >= row()) {
				std::__throw_out_of_range_fmt(
					__N("matrix::row_at: _n "
						"(which is %zu) >= this->row() "
						"(which is %zu)"),
					_n, row());
			}
			return row(_n);
		}
		inline row_vector at(size_t _n) { return row_at(_n); }
		inline const_row_vector at(size_t _n) const { return row_at(_n); }
		inline col_vector col(size_t _m) { return {_m, *this}; }
		inline const_col_vector col(size_t _m) const { return {_m, *this}; }
		inline col_vector col_at(size_t _m) {
			if (_m >= col()) {
				if (_m >= col()) {
					std::__throw_out_of_range_fmt(
						__N("matrix::col_at: _m "
							"(which is %zu) >= this->col() "
							"(which is %zu)"),
						_m, col());
				}
			}
			return col(_m);
		}
		inline const_col_vector col_at(size_t _m) const {
			if (_m >= col()) {
				if (_m >= col()) {
					std::__throw_out_of_range_fmt(
						__N("matrix::col_at: _m "
							"(which is %zu) >= this->col() "
							"(which is %zu)"),
						_m, col());
				}
			}
			return col(_m);
		}
		inline row_vector operator[](size_t _n) { return row(_n); }
		inline const_row_vector operator[](size_t _n) const { return row(_n); }

		bool iszero() const {
			elem_t zero_elem = elem_t();
			for (const auto &row : __mat) {
				for (const auto &e : row) {
					if (e != zero_elem) {
						return false;
					}
				}
			}
			return true;
		}
		inline bool isscalar() const { return (row() == 1 && col() == 1); }
		inline bool isrowvector() const { return (row() == 1); }
		inline bool iscolvector() const { return (col() == 1); }
		inline bool isvector() const { return (row() == 1 || col() == 1); }
		bool issquare() const { return (row() == col()); }

		inline void clear() {
			__mat.clear();
		}
		inline void resize(size_t _n, size_t _m) {
			if (!_n) {
				_m = 0;
			}
			__mat.resize(_n);
			for (auto &r : __mat) {
				r.resize(_m);
			}
		}
		inline matrix &assign() {
			clear();
			return *this;
		}
		inline matrix &assign(size_t _n, size_t _m) {
			resize(_n, _m);
			return *this;
		}
		inline matrix &assign(
			std::initializer_list<std::initializer_list<elem_t>> _mat) {
			__mat.resize(_mat.size());
			size_t m = 0, i = 0;
			for (auto &r : _mat) {
				__mat[i++] = r;
				if (r.size() > m) {
					m = r.size();
				}
			}
			for (auto &r : __mat) {
				r.resize(m);
			}
			if (!m) {
				__mat.clear();
			}
			return *this;
		}
		inline matrix &assign(const std::vector<std::vector<elem_t>> &_mat) {
			__mat = _mat;
			size_t m = 0;
			for (const auto &r : _mat) {
				if (r.size() > m) {
					m = r.size();
				}
			}
			if (m) {
				for (auto &r : __mat) {
					r.resize(m);
				}
			} else {
				__mat.clear();
			}
			return *this;
		}

		inline matrix transpose() const {
			size_t n = row(), m = col();
			matrix t = matrix(m, n);
			for (size_t i = 0, j; i != n; ++i) {
				for (j = 0; j != m; ++j) {
					t.__mat[j][i] = __mat[i][j];
				}
			}
			return t;
		}
		inline matrix &totranspose() {
			return (*this = transpose());
		}
		inline static matrix transpose(const matrix &_mat) {
			return _mat.transpose();
		}

		inline matrix &plus_equal(const matrix &_rhs) {
			__size_check(_rhs);
			if (__mat.size()) {
				size_t n = __mat.size(), m = __col();
				for (size_t i = 0; i != n; ++i) {
					for (size_t j = 0; j != m; ++j) {
						__mat[i][j] = __plus(__mat[i][j], _rhs.__mat[i][j]);
					}
				}
			}
			return *this;
		}
		inline static matrix plus(matrix _lhs, const matrix &_rhs) {
			return _lhs.plus_equal(_rhs);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline matrix &multiplies_equal(const scalar_t &_scalar) {
			scalar_multiplies_t mul;
			for (auto &r : __mat) {
				for (auto &e : r) {
					e = mul(_scalar, e);
				}
			}
			return *this;
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline static matrix multiplies(const scalar_t &_scalar, matrix _mat) {
			return _mat.multiplies_equal(_scalar);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline static matrix multiplies(matrix _mat, const scalar_t &_scalar) {
			return _mat.multiplies_equal(_scalar);
		}
		inline matrix &minus_equal(matrix _rhs) {
			return this->plus_equal(_rhs.multiplies_equal(-1));
		}
		inline static matrix minus(matrix _lhs, const matrix &_rhs) {
			return _lhs.minus_equal(_rhs);
		}
		inline static multiplies(const matrix &_lhs, const matrix &_rhs) {
			plus_t plus;
			multiplies_t mul;
			size_t n = _lhs.col(), m = _rhs.col();
			matrix res(n, m);
			for (size_t i = 0, j, k; i != n; ++i) {
				for (j = 0; j != m; ++j) {
					res.__mat[i][j] = plus(res.__mat[i][j],
										   mul(_lhs.__mat[i][k],
											   _rhs.__mat[k][j]));
				}
			}
			return res;
		}
		inline matrix &multiplies_equal(const matrix &_rhs) {
			return (*this = multiplies(*this, _rhs));
		}
		inline matrix &operator+=(const matrix &_rhs) {
			return this->plus_equal(_rhs);
		}
		inline friend matrix &operator+(const matrix &_lhs,
										const matrix &_rhs) {
			return plus(_lhs, _rhs);
		}
		inline matrix &operator-=(const matrix &_rhs) {
			return this->minus_equal(_rhs);
		}
		inline friend matrix operator-(const matrix &_lhs,
									   const matrix &_rhs) {
			return minus(_lhs, _rhs);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline friend matrix operator*=(const scalar_t &_lhs, matrix _rhs) {
			return _rhs.multiplies_equal<scalar_t, scalar_multiplies_t>(_lhs);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline matrix &operator*=(const scalar_t _rhs) {
			return this->multiplies_equal<scalar_t, scalar_multiplies_t>(_rhs);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline friend matrix operator*(const scalar_t &_lhs,
									   const matrix &_rhs) {
			return multiplies<scalar_t, scalar_multiplies_t>(_lhs, _rhs);
		}
		template <typename scalar_t,
				  typename scalar_multiplies_t =
					  scalar_multiplies<scalar_t, elem_t>>
		inline friend matrix operator*(const matrix &_lhs,
									   const scalar_t &_rhs) {
			return multiplies<scalar_t, scalar_multiplies_t>(_lhs, _rhs);
		}
		inline friend matrix operator*(const matrix &_lhs,
									   const matrix &_rhs) {
			return multiplies(_lhs, _rhs);
		}

	protected:
		plus_t __plus;
		multiplies_t __multiplies;

		std::vector<std::vector<elem_t>> __mat;

		inline size_t __col() const { return __mat[0].size(); }

		inline void __size_check(const matrix &_mat) {
			if (_mat.row() != row() || (row() && _mat.__col() != __col())) {
				std::__throw_out_of_range_fmt(
					__N("matrix::__size_check: _mat.size() "
						"(which is %zu*%zu) != this->size() "
						"(which is %zu*%zu)"),
					row(), __col(), _mat.row(), _mat.__col());
			}
		}

	private:
	};
}

#endif