#ifndef _GROUP_HPP
#define _GROUP_HPP

namespace ds {
	template <typename scalar_t, typename type_t>
	struct scalar_multiplies {
		inline type_t operator()(const scalar_t &_lhs, const type_t &_rhs) {
			return (_lhs * _rhs);
		}
	};
}

#endif