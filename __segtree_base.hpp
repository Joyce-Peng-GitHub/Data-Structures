#ifndef __SEGTREE_BASE_HPP
#define __SEGTREE_BASE_HPP

#include <cstddef>
#include <vector>

namespace ds {
	template <typename elem_t,
			  typename oper_t,
			  elem_t init>
	class __segtree_base {
	public:
		__segtree_base() = default;
		__segtree_base(size_t _n) : __tree((_n << 2) - 1, init) {}
		template <typename iter_t>
		__segtree_base(iter_t _begin, iter_t _end)
			: __tree((std::distance(_begin, _end) << 2) - 1) {
			this->__build(0, _begin, _end);
		}

		inline size_t treesize() const { return this->__tree.size(); }
		inline size_t size() const { return ((this->__tree.size() + 1) >> 2); }

		inline void build(size_t _n) {
			this->__tree = std::move(std::vector<elem_t>((_n << 2) - 1, init));
		}
		inline void build(size_t _n, const elem_t &_val) {
			this->__tree = std::move(std::vector<elem_t>((_n << 2) - 1, _val));
			this->__up(0, 0, _n);
		}
		template <typename iter_t>
		inline void build(iter_t _begin, iter_t _end) {
			this->build(std::distance(_begin, _end));
			this->__build(0, _begin, _end);
		}
		inline void modify(size_t _pos, const elem_t &_val) {
			this->__range_check(_pos);
			this->__modify(0, 0, this->size(), _pos, _val);
		}
		inline elem_t query(size_t _pos = 0, size_t _n = 1ull) const {
			this->__range_check(_pos);
			return this->__query(0, 0, this->size(), _pos,
								 std::min(_n, this->size() - _pos));
		}

	protected:
		std::vector<elem_t> __tree;
		oper_t __oper;

		inline void __range_check(size_t _pos) const {
			if (_pos >= this->__tree.size()) {
				std::__throw_out_of_range_fmt(__N("segtree::__range_check: _pos "
												  "(which is %zu) >= this->size() "
												  "(which is %zu)"),
											  _pos, this->size());
			}
		}
		inline static size_t __lchild(size_t _index) { return ((_index << 1) ^ 1); }
		inline static size_t __rchild(size_t _index) { return ((_index + 1) << 1); }
		virtual void __up(size_t _index, size_t _begin, size_t _end) {
			if (_begin + 1 != _end) {
				size_t mid = _begin + ((_end - _begin) >> 1),
					   lch = __lchild(_index), rch = __rchild(_index);
				this->__up(lch, _begin, mid);
				this->__up(rch, mid, _end);
				this->__tree[_index] = this->__oper(this->__tree[lch],
													this->__tree[rch]);
			}
		}
		template <typename iter_t>
		virtual void __build(size_t _index, iter_t _begin, iter_t _end) {
			size_t n = std::distance(_begin, _end);
			if (n == 1) {
				this->__tree[_index] = *_begin;
			} else {
				iter_t mid = std::next(_begin, (n >> 1));
				size_t lch = __lchild(_index), rch = __rchild(_index);
				this->__build(lch, _begin, mid);
				this->__build(rch, mid, _end);
				this->__tree[_index] = this->__oper(this->__tree[lch],
													this->__tree[rch]);
			}
		}
		virtual void __modify(size_t _index, size_t _begin, size_t _end,
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
		virtual elem_t __query(size_t _index, size_t _begin, size_t _end,
							   size_t _pos, size_t _n) const {
			if (_pos + _n <= _begin || _pos >= _end) {
				return init;
			}
			if (_begin + 1 == _end) {
				return this->__tree[_index];
			}
			size_t mid = _begin + ((_end - _begin) >> 1);
			return this->__oper(this->__query(__lchild(_index), _begin, mid, _pos, _n),
								this->__query(__rchild(_index), mid, _end, _pos, _n));
		}

	private:
	};
}

#endif