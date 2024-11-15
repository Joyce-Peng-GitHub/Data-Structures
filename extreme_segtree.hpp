#ifndef _EXTREME_SEGTREE_HPP
#define _EXTREME_SEGTREE_HPP

#include "generic_segtree.hpp"

#include <functional>

namespace ds {
	template <typename elem_t,
			  elem_t init,
			  typename comp_t = std::less<elem_t>>
	using extreme_segtree = generic_segtree<elem_t, init, comp_t>;
	template <typename elem_t, elem_t type_max>
	using min_segtree = extreme_segtree<elem_t, type_max, std::less<elem_t>>;
	template <typename elem_t, elem_t type_min>
	using max_segtree = extreme_segtree<elem_t, type_min, std::greater<elem_t>>;
}

#endif