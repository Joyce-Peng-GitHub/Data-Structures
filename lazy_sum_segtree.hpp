#ifndef _LAZY_SUM_SEGTREE_HPP
#define _LAZY_SUM_SEGTREE_HPP

#include "__segtree_base.hpp"

namespace ds {
	template <typename elem_t,
			  elem_t init,
			  typename plus_t = std::plus<elem_t>>
	class lazy_sum_segtree
		: public __segtree_base<elem_t, init, oper_t> {
	public:
		using base_t = __segtree_base<elem_t, init, oper_t>;

		lazy_sum_segtree() = default;
		lazy_sum_segtree(size_t _n) : base_t(_n), __lazy(this->treesize()) {}
		template <typename iter_t>
		lazy_sum_segtree(iter_t _begin, iter_t _end)
			: base_t(_begin, _end), __lazy(this->treesize()) {}

		inline void clear() {
			this->__tree.clear();
			this->__lazy.clear();
		}
		inline void build(size_t _n) {
			size_t old = this->treesize();
			this->__tree.resize(((_n << 2) + 1), init);
			this->__lazy.resize(this->treesize());
			for (size_t i = 0; i != old && i != this->treesize(); ++i) {
				this->__tree[i] = init;
				this->__lazy[i] = false;
			}
		} 

	protected:
		using base_t::__lchild;
		using base_t::__rchild;

		std::vector<bool> __lazy;

	private:
	};
}

#endif