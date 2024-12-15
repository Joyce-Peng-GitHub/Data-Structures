#ifndef _EXTREME_SEGTREE_HPP
#define _EXTREME_SEGTREE_HPP

#include "__segtree_base.hpp"

#include <functional>

namespace ds {
	template <typename elem_t,
			  elem_t init,
			  typename comp_t = std::less<elem_t>>
	using extreme_segtree = __segtree_base<elem_t, init, comp_t>;
	template <typename elem_t, elem_t type_max>
	using min_segtree = extreme_segtree<elem_t, type_max, std::less<elem_t>>;
	template <typename elem_t, elem_t type_min>
	using max_segtree = extreme_segtree<elem_t, type_min, std::greater<elem_t>>;
}

#endif