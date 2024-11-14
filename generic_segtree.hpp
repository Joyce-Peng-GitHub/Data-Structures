#ifndef _GENERIC_SEGTREE_HPP
#define _GENERIC_SEGTREE_HPP

// #include "__segtree_base.hpp"

namespace ds {
	template <typename elem_t,
			  typename oper_t,
			  elem_t init>
	class generic_segtree
		: public __segtree_base<elem_t, oper_t, init> {
	public:
		using base_t = __segtree_base<elem_t, oper_t, init>;

		generic_segtree() = default;
		generic_segtree(size_t _n) : base_t(_n) {}
		template <typename iter_t>
		generic_segtree(iter_t _begin, iter_t _end)
			: base_t(_begin, _end) {
		}

		inline void modify(size_t _pos, const elem_t &_val) {
			this->__range_check(_pos);
			this->__modify(0, 0, this->size(), _pos, _val);
		}
		inline elem_t query(size_t _pos = 0, size_t _n = 1ull) const {
			this->__range_check(_pos);
			return this->__query(0, 0, this->size(), _pos,
								 _pos + std::min(_n, this->size() - _pos));
		}

	protected:
		using base_t::__lchild;
		using base_t::__rchild;

		oper_t __oper;

		void __modify(size_t _index, size_t _begin, size_t _end,
					  size_t _pos, const elem_t &_val) {
			if (_pos < _begin || _pos >= _end) {
				return;
			}
			if (_begin + 1 == _end) {
				this->__tree[_index] = _val;
			} else {
				size_t mid = _begin + ((_end - _begin) >> 1),
					   lch = __lchild(_index), rch = __rchild(_index);
				if (_pos < mid) {
					this->__modify(lch, _begin, mid, _pos, _val);
				} else {
					this->__modify(rch, mid, _end, _pos, _val);
				}
				this->__tree[_index] = this->__oper(this->__tree[lch],
													this->__tree[rch]);
			}
		}
		elem_t __query(size_t _index, size_t _node_begin, size_t _node_end,
					   size_t _query_begin, size_t _query_end) const {
			if (_query_begin < _node_begin) {
				_query_begin = _node_begin;
			}
			if (_query_end > _node_end) {
				_query_end = _node_end;
			}
			if (_query_begin >= _query_end) {
				return init;
			}
			if (_query_begin == _node_begin && _query_end == _node_end) {
				return this->__tree[_index];
			}
			size_t node_mid = _node_begin + ((_node_end - _node_begin) >> 1);
			return this->__oper(this->__query(__lchild(_index),
											  _node_begin,
											  node_mid,
											  _query_begin, _query_end),
								this->__query(__rchild(_index),
											  node_mid, _node_end,
											  _query_begin, _query_end));
		}

	private:
	};
}

#endif