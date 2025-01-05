#ifndef _FENWICKTREE_HPP
#define _FENWICKTREE_HPP

#include <cstddef>
#include <vector>

#include "ds_internal.hpp"

namespace ds {
	using std::size_t;

	template <typename T, typename Oper = std::plus<T>, T id_elem = T()>
	class fenwicktree {

	public:
		inline static size_t lowbit(size_t x) { return (x & (-x)); }

		inline explicit fenwicktree(size_t n = 0) : m_tree(n + 1, id_elem) {}
		inline fenwicktree(size_t n, const T &value) : m_tree(n + 1, value) {
			m_tree.front() = id_elem;
			m_build();
		}
		template <typename Iter, typename = internal::RequireInputIter<Iter>>
		inline fenwicktree(Iter begin, Iter end)
			: m_tree(std::distance(begin, end) + 1) {
			m_tree.front() = id_elem;
			std::copy(begin, end, m_tree.begin() + 1);
			m_build();
		}

		inline size_t tree_size() const { return m_tree.size(); }
		inline size_t size() const { return (m_tree.size() - 1); }

		inline void assign(std::initializer_list<T> list) {
			assign(list.begin(), list.end());
		}
		inline void assign(size_t n = 0) { m_tree.assign(n + 1, id_elem); }
		inline void assign(size_t n, const T &value) {
			m_tree.assign(n + 1, value);
			m_tree.front() = id_elem;
			m_build();
		}
		template <typename Iter, typename = internal::RequireInputIter<Iter>>
		inline void assign(Iter begin, Iter end) {
			m_tree.resize(std::distance(begin, end) + 1);
			std::copy(begin, end, m_tree.begin() + 1);
			m_build();
		}
		inline void modify(size_t index, const T &diff) {
			m_range_check(index);
			for (++index; index < m_tree.size(); index += lowbit(index)) {
				m_tree[index] = m_oper(m_tree[index], diff);
			}
		}
		inline T query(size_t n = -1) const {
			if (n >= m_tree.size()) {
				n = m_tree.size() - 1;
			}
			T res = id_elem;
			for (; n; n -= lowbit(n)) {
				res = m_oper(res, m_tree[n]);
			}
			return res;
		}
		inline T operator[](size_t n) const {
			return query(n);
		}

	protected:
		std::vector<T> m_tree;
		Oper m_oper;

		inline void m_range_check(size_t index) const {
			if (index + 1 >= m_tree.size()) {
				std::__throw_out_of_range_fmt(__N("fenwicktree::__range_check: index "
												  "(which is %zu) >= this->size() "
												  "(which is %zu)"),
											  index, m_tree.size() - 1);
			}
		}
		inline void m_build() {
			for (size_t i = 1, j; i < m_tree.size(); ++i) {
				j = i + lowbit(i);
				if (j < m_tree.size()) {
					m_tree[j] = m_oper(m_tree[j], m_tree[i]);
				}
			}
		}
	};
}

#endif // _FENWICKTREE_HPP