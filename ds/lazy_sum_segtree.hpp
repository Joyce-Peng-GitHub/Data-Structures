#ifndef _LAZY_SUM_SEGTREE_HPP
#define _LAZY_SUM_SEGTREE_HPP

#include "group.hpp"
#include "sum_segtree.hpp"

namespace ds {
	template <typename elem_t,
			  elem_t init = elem_t(),
			  typename plus_t = std::plus<elem_t>,
			  typename scalar_multiplies_t = scalar_multiplies<size_t, elem_t>>
	class lazy_sum_segtree
		: public sum_segtree<elem_t, init, plus_t> {
	public:
		using base_t = sum_segtree<elem_t, init, plus_t>;

		lazy_sum_segtree() = default;
		lazy_sum_segtree(size_t _n)
			: base_t(_n), __lazy(_n ? ((_n << 1) - 1) : 0) {}
		template <typename iter_t>
		lazy_sum_segtree(iter_t _begin, iter_t _end)
			: base_t(_begin, _end),
			  __lazy((this->size() ? ((this->size() << 1) - 1) : 0), init) {}

		inline size_t lazysize() const { return this->__lazy.size(); }

		inline void clear() {
			this->__tree.clear();
			this->__lazy.clear();
		}
		inline void build(size_t _n) {
			if (_n) {
				this->__build(_n);
			} else {
				this->clear();
			}
		}
		inline void build(size_t _n, const elem_t &_val) {
			if (_n) {
				this->__build(_n);
				this->__add(0, 0, _n, 0, _n, _val);
			} else {
				this->clear();
			}
		}
		template <typename iter_t>
		inline void build(iter_t _begin, iter_t _end) {
			size_t m = this->lazysize(), n = std::distance(_begin, _end);
			if (n) {
				this->__lazy.resize(((n << 1) - 1), init);
				for (size_t i = 0; i != m && i != this->lazysize(); ++i) {
					this->__lazy[i] = init;
				}
				this->build(n);
				this->base_t::__build(0, _begin, n);
			} else {
				this->clear();
			}
		}

		/**
		 * @return difference between the original value and the new
		 */
		inline elem_t modify(size_t _pos, const elem_t &_val) {
			this->__range_check(_pos);
			return this->__modify(0, 0, this->size(), _pos, _val);
		}
		inline void add(size_t _pos, size_t _n, const elem_t &_diff) {
			this->__range_check(_pos);
			if (_n) {
				this->__add(0, 0, this->size(),
							_pos, std::min(_pos + _n, this->size()),
							_diff);
			}
		}
		inline elem_t query(size_t _pos, size_t _n = 1) {
			this->__range_check(_pos);
			return (_n ? this->__query(0, 0, this->size(), _pos,
									   std::min(_pos + _n, this->size()))
					   : init);
		}

	protected:
		using base_t::__lchild;
		using base_t::__rchild;

		std::vector<elem_t> __lazy; // leaf nodes don't need lazy tags
		scalar_multiplies_t __mul;	// scalar multiplication functor

		inline void __spread(size_t _index, size_t _begin, size_t _end) {
			if (this->__lazy[_index] != init) {
				size_t lch = __lchild(_index), rch = __rchild(_index);
				/* Update the 2 sub-nodes */
				this->__tree[lch] =
					this->__oper(this->__tree[lch],
								 this->__mul(((_end - _begin) >> 1),
											 this->__lazy[_index]));
				this->__tree[__rchild(_index)] =
					this->__oper(this->__tree[rch],
								 this->__mul(((_end - _begin + 1) >> 1),
											 this->__lazy[_index]));
				/* Tag the 2 sub-nodes */
				if (lch < this->lazysize()) {
					this->__lazy[lch] = this->__oper(this->__lazy[lch],
													 this->__lazy[_index]);
				}
				if (rch < this->lazysize()) {
					this->__lazy[rch] = this->__oper(this->__lazy[rch],
													 this->__lazy[_index]);
				}
				/* Untag the current node */
				this->__lazy[_index] = init;
			}
		} // assumes _index is not a leaf node
		inline void __build(size_t _n) {
			size_t m = this->treesize(); // old tree size
			this->__tree.resize((_n << 2) - 1, init);
			this->__lazy.resize((_n << 1) - 1, init);
			for (size_t i = 0; i != m && i != this->treesize(); ++i) {
				this->__tree[i] = init;
			}
			m = ((m + 1) >> 1) - 1; // old lazy size
			for (size_t i = 0; i != m && i != this->lazysize(); ++i) {
				this->__lazy[i] = init;
			}
		}
		void __modify(size_t _index, size_t _begin, size_t _end,
					  size_t _pos, const elem_t &_val) {
			if (_begin + 1 == _end) {
				this->__tree[_index] = _val;
				return;
			} // _index is a leaf node
			/**
			 * If _index is a leaf node,
			 * it must be covered entirely by the target range.
			 */
			this->__spread(_index, _begin, _end);
			size_t mid = _begin + ((_end - _begin) >> 1);
			if (_pos < mid) {
				this->__modify(__lchild(_index), _begin, mid,
							   _pos, _val);
			} else {
				this->__modify(__rchild(_index), mid, _end,
							   _pos, _val);
			}
			this->__tree[_index] = this->__oper(this->__tree[__lchild(_index)],
												this->__tree[__rchild(_index)]);
		}
		void __add(size_t _index, size_t _node_begin, size_t _node_end,
				   size_t _begin, size_t _end, const elem_t &_diff) {
			if (_begin <= _node_begin && _end >= _node_end) {
				/* The entire segment is covered by the target range. */
				this->__tree[_index] =
					this->__oper(this->__tree[_index],
								 this->__mul(_node_end - _node_begin, _diff));
				if (_node_begin + 1 != _node_end) {
					this->__lazy[_index] = this->__oper(this->__lazy[_index],
														_diff);
				} // not a leaf node
			} else {
				/**
				 * If _index is a leaf node,
				 * it must be covered entirely by the target range.
				 */
				this->__spread(_index, _node_begin, _node_end);
				size_t node_mid = _node_begin +
								  ((_node_end - _node_begin) >> 1);
				if (_begin < node_mid) {
					this->__add(__lchild(_index), _node_begin, node_mid,
								_begin, _end, _diff);
				}
				if (_end > node_mid) {
					this->__add(__rchild(_index), node_mid, _node_end,
								_begin, _end, _diff);
				}
				this->__tree[_index] =
					this->__oper(this->__tree[__lchild(_index)],
								 this->__tree[__rchild(_index)]);
			}
		}
		elem_t __query(size_t _index, size_t _node_begin, size_t _node_end,
					   size_t _begin, size_t _end) {
			if (_begin <= _node_begin && _end >= _node_end) {
				/* The entire segment is covered by the target range. */
				return this->__tree[_index];
			}
			/**
			 * If _index is a leaf node,
			 * it must be covered entirely by the target range.
			 */
			this->__spread(_index, _node_begin, _node_end);
			size_t node_mid = _node_begin +
							  ((_node_end - _node_begin) >> 1);
			return this->__oper((_begin < node_mid)
									? this->__query(__lchild(_index),
													_node_begin, node_mid,
													_begin, _end)
									: init,
								(_end > node_mid)
									? this->__query(__rchild(_index),
													node_mid, _node_end,
													_begin, _end)
									: init);
		}

	private:
	};
}

#endif