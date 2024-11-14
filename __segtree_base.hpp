#ifndef __SEGTREE_BASE_HPP
#define __SEGTREE_BASE_HPP

#include <cstddef>
#include <vector>

namespace ds {
	/**
	 * @param elem_t Type of the elements
	 * @param oper_t The functor equipped with operation() overloaded,
	 * 	which should satisfy interval additivity
	 * @param init The initial value of oper_t and identity element
	 */
	template <typename elem_t, typename oper_t, elem_t init>
	class __segtree_base {
	public:
		inline __segtree_base() = default;
		inline __segtree_base(size_t _n) : __tree(((_n << 2) + 1), init) {}
		template <typename iter_t>
		__segtree_base(iter_t _begin, iter_t _end) : __tree(std::distance(_begin, _end)) {
			this->__build(0, _begin, _end);
		}

		inline size_t treesize() const { return this->__tree.size(); }
		inline size_t size() const { return ((this->treesize() + 1) >> 2); }

		inline void clear() { this->__tree.clear(); }
		inline void build(size_t _n) {
			size_t old = this->treesize();
			this->__tree.resize(((_n << 2) + 1), init);
			for (size_t i = 0; i != old && i != this->treesize(); ++i) {
				this->__tree[i] = init;
			}
		}
		inline void build(size_t _n, const elem_t &_val) {
			size_t old = this->treesize();
			this->__tree.resize(((_n << 2) + 1), _val);
			for (size_t i = 0; i != old && i != this->treesize(); ++i) {
				this->__tree[i] = _val;
			}
			this->__up(0, 0, _n);
		}
		template <typename iter_t>
		inline void build(iter_t _begin, iter_t _end) {
			this->build(std::distance(_begin, _end));
			this->__build(0, _begin, _end);
		}

	protected:
		std::vector<elem_t> __tree;

		inline void __range_check(size_t _pos) const {
			if (_pos >= this->__tree.size()) {
				std::__throw_out_of_range_fmt(__N("__segtree_base::__range_check: _pos "
												  "(which is %zu) >= this->size() "
												  "(which is %zu)"),
											  _pos, this->size());
			}
		}
		inline static size_t __lchild(size_t _index) { return ((_index << 1) ^ 1); }
		inline static size_t __rchild(size_t _index) { return ((_index + 1) << 1); }

		void __up(size_t _index, size_t _begin, size_t _end) {
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
		void __build(size_t _index, iter_t _begin, iter_t _end) {
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

	private:
	};
}

#endif