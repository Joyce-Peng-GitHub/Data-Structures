#ifndef _SUM_SEGTREE_HPP
#define _SUM_SEGTREE_HPP

#include "__segtree_base.hpp"

#include <functional>

namespace ds {
	template <typename elem_t,
			  elem_t init = elem_t(),
			  typename plus_t = std::plus<elem_t>>
	class sum_segtree : public __segtree_base<elem_t, init, plus_t> {
	public:
		using base_t = __segtree_base<elem_t, init, plus_t>;

		sum_segtree() = default;
		sum_segtree(size_t _n) : base_t(_n) {}
		template <typename iter_t>
		sum_segtree(iter_t _begin, iter_t _end) : base_t(_begin, _end) {}

		inline void add(size_t _pos, const elem_t &_diff) {
			this->__range_check(_pos);
			this->__add(0, 0, this->size(), _pos, _diff);
		}

	protected:
		using base_t::__lchild;
		using base_t::__rchild;

		void __add(size_t _index, size_t _begin, size_t _end,
				   size_t _pos, const elem_t &_diff) {
			this->__tree[_index] = this->__oper(this->__tree[_index], _diff);
			if (_begin + 1 != _end) {
				size_t mid = _begin + ((_end - _begin) >> 1);
				if (_pos < mid) {
					this->__add(__lchild(_index), _begin, mid, _pos, _diff);
				} else {
					this->__add(__rchild(_index), mid, _end, _pos, _diff);
				}
			}
		}

	private:
	};
}

#endif