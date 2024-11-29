#ifndef ___SEGTREE_BASE_HPP
#define ___SEGTREE_BASE_HPP

#include <cstddef>
#include <vector>

namespace ds {
	template <typename elem_t,
			  elem_t init,
			  typename oper_t>
	class __segtree_base {
	public:
		inline __segtree_base() = default;
		inline __segtree_base(size_t _n) : __tree((_n ? ((_n << 2) - 1) : 0), init) {}
		template <typename iter_t>
		__segtree_base(iter_t _begin, iter_t _end) {
			size_t n = std::distance(_begin, _end);
			if (n) {
				__tree.resize((n << 2) - 1);
				__build(0, _begin, n);
			}
		}

		inline size_t treesize() const { return __tree.size(); }
		inline size_t size() const { return ((treesize() + 1) >> 2); }

		inline void clear() { __tree.clear(); }
		inline void build(size_t _n) {
			if (_n) {
				size_t old = treesize();
				__tree.resize(((_n << 2) - 1), init);
				for (size_t i = 0; i != old && i != treesize(); ++i) {
					__tree[i] = init;
				}
			} else {
				clear();
			}
		}
		inline void build(size_t _n, const elem_t &_val) {
			if (_n) {
				size_t old = treesize();
				__tree.resize(((_n << 2) - 1), _val);
				for (size_t i = 0; i != old && i != treesize(); ++i) {
					__tree[i] = _val;
				}
				__up(0, 0, _n);
			} else {
				clear();
			}
		}
		template <typename iter_t>
		inline void build(iter_t _begin, iter_t _end) {
			size_t n = std::distance(_begin, _end);
			if (n) {
				build(n);
				__build(0, _begin, n);
			} else {
				clear();
			}
		}

		inline void modify(size_t _pos, const elem_t &_val) {
			__range_check(_pos);
			__modify(0, 0, size(), _pos, _val);
		}
		inline elem_t query(size_t _pos = 0, size_t _n = 1ull) const {
			__range_check(_pos);
			return __query(0, 0, size(), _pos,
						   _pos + std::min(_n, size() - _pos));
		}

	protected:
		std::vector<elem_t> __tree;
		oper_t __oper;

		inline void __range_check(size_t _pos) const {
			if (_pos >= __tree.size()) {
				std::__throw_out_of_range_fmt(__N("__segtree_base::__range_check: _pos "
												  "(which is %zu) >= this->size() "
												  "(which is %zu)"),
											  _pos, size());
			}
		}
		inline static size_t __lchild(size_t _index) { return ((_index << 1) ^ 1); }
		inline static size_t __rchild(size_t _index) { return ((_index + 1) << 1); }

		void __up(size_t _index, size_t _begin, size_t _end) {
			if (_begin + 1 != _end) {
				size_t mid = _begin + ((_end - _begin) >> 1),
					   lch = __lchild(_index), rch = __rchild(_index);
				__up(lch, _begin, mid);
				__up(rch, mid, _end);
				__tree[_index] = __oper(__tree[lch],
										__tree[rch]);
			}
		}
		template <typename iter_t>
		void __build(size_t _index, iter_t _begin, size_t _n) {
			if (_n == 1) {
				__tree[_index] = *_begin;
			} else {
				size_t lch = __lchild(_index), rch = __rchild(_index);
				__build(lch, _begin, (_n >> 1));
				__build(rch,
						std::next(_begin, _n >> 1),
						(_n >> 1) + (_n & 1));
				__tree[_index] = __oper(__tree[lch],
										__tree[rch]);
			}
		}
		void __modify(size_t _index, size_t _begin, size_t _end,
					  size_t _pos, const elem_t &_val) {
			if (_pos < _begin || _pos >= _end) {
				return;
			}
			if (_begin + 1 == _end) {
				__tree[_index] = _val;
			} else {
				size_t mid = _begin + ((_end - _begin) >> 1),
					   lch = __lchild(_index), rch = __rchild(_index);
				if (_pos < mid) {
					__modify(lch, _begin, mid, _pos, _val);
				} else {
					__modify(rch, mid, _end, _pos, _val);
				}
				__tree[_index] = __oper(__tree[lch],
										__tree[rch]);
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
				return __tree[_index];
			}
			size_t node_mid = _node_begin + ((_node_end - _node_begin) >> 1);
			return __oper(__query(__lchild(_index),
								  _node_begin,
								  node_mid,
								  _query_begin, _query_end),
						  __query(__rchild(_index),
								  node_mid, _node_end,
								  _query_begin, _query_end));
		}

	private:
	};
}

#endif