#ifndef _FENWICK_TREE_HPP
#define _FENWICK_TREE_HPP

#include <cstddef>
#include <vector>

namespace ds {
	template <typename elem_t,
			  typename oper_t = std::plus<elem_t>,
			  typename inv_t = std::minus<elem_t>,
			  elem_t init = elem_t()>
	class fenwicktree {
	public:
		inline static size_t lowbit(size_t x) { return (x & (-x)); }

		inline fenwicktree(size_t _n = 0) : __tree(_n + 1, init) {}
		template <typename iter_t>
		inline fenwicktree(iter_t _begin, iter_t _end)
			: __tree(std::distance(_begin, _end) + 1) {
			this->__tree.front() = init;
			std::copy(_begin, _end, this->__tree.begin() + 1);
			this->__build();
		}

		inline size_t treesize() const { return this->__tree.size(); }
		inline size_t size() const { return (this->__tree.size() - 1); }
		template <typename iter_t>
		inline void build(iter_t _begin, iter_t _end) {
			this->__tree.resize(std::distance(_begin, _end) + 1);
			std::copy(_begin, _end, this->__tree.begin() + 1);
			this->__build();
		}
		inline void add(size_t _index, const elem_t &_diff) {
			this->__range_check(_index);
			for (++_index; _index < this->__tree.size(); _index += lowbit(_index)) {
				this->__tree[_index] = this->__oper(this->__tree[_index], _diff);
			}
		}
		inline void subtract(size_t _index, const elem_t &_diff) {
			this->__range_check(_index);
			for (++_index; _index < this->__tree.size(); _index += lowbit(_index)) {
				this->__tree[_index] = this->__inv(this->__tree[_index], _diff);
			}
		}
		inline elem_t prefix_sum(size_t _n) const {
			elem_t res = init;
			for (; _n; _n -= lowbit(_n)) {
				res = this->__oper(res, this->__tree[_n]);
			}
			return res;
		}
		inline elem_t range_sum(size_t _index, size_t _n) const {
			return this->__inv(this->prefix_sum(_index + _n), this->prefix_sum(_index));
		}

	protected:
		std::vector<elem_t> __tree;
		oper_t __oper;
		inv_t __inv;

		inline void __range_check(size_t _index) const {
			if (_index + 1 >= this->__tree.size()) {
				std::__throw_out_of_range_fmt(__N("fenwicktree::__range_check: _index "
												  "(which is %zu) >= this->size() "
												  "(which is %zu)"),
											  _index, this->size());
			}
		}
		inline void __build() {
			for (size_t i = 1, j; i != this->__tree.size(); ++i) {
				j = i + lowbit(i);
				if (j < this->__tree.size()) {
					this->__tree[j] = this->__oper(this->__tree[j], this->__tree[i]);
				}
			}
		}

	private:
	};
}

#endif