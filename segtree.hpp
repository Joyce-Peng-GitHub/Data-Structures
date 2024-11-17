#ifndef _SEGTREE_HPP
#define _SEGTREE_HPP

#include "__segtree_base.hpp"
#include "extreme_segtree.hpp"
#include "sum_segtree.hpp"

namespace ds {
	template <typename elem_t,
			  elem_t init,
			  typename oper_t>
	struct __segtree_helper {
		using tree_t = __segtree_base<elem_t, init, oper_t>;
	};

	template <typename elem_t, elem_t init>
	struct __segtree_helper<elem_t, init, std::plus<elem_t>> {
		using tree_t = sum_segtree<elem_t, init, std::plus<elem_t>, std::minus<elem_t>>;
	};
	template <typename elem_t, elem_t init>
	struct __segtree_helper<elem_t, init, std::multiplies<elem_t>> {
		using tree_t = sum_segtree<elem_t, init, std::multiplies<elem_t>, std::divides<elem_t>>;
	};

	template <typename elem_t,
			  elem_t init,
			  typename oper_t>
	using segtree = typename __segtree_helper<elem_t, init, oper_t>::tree_t;
}

#endif