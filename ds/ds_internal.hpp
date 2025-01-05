#ifndef _DS_INTERNAL_HPP
#define _DS_INTERNAL_HPP

#include <iterator>

namespace ds {
	namespace internal {
		template <typename Iter>
		using RequireInputIter =
			std::enable_if_t<
				std::is_convertible<
					std::iterator_traits<Iter>::iterator_category,
					std::input_iterator_tag>::value>;
	}
}

#endif // _DS_INTERNAL_HPP