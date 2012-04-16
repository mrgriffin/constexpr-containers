#ifndef CEXPR_ALGORITHM_HPP
#define CEXPR_ALGORITHM_HPP

#include <cstddef>
#include <functional> // TODO: Is std::less constexpr?
#include "cexpr/detail/basic_list.hpp"

namespace cexpr {

/*!
 * Sorts the elements in \p container using <=.
 */
template<class Container>
constexpr Container sort(Container const& container);

/*!
 * Sorts the elements in \p container using \p cmp.
 */
template<class Container, class Compare>
constexpr Container sort(Container const& container, Compare cmp);

namespace detail {

/*
 * if (N > 0 && M > 0)
 *     if (list1[0] <= list2[0])
 *         return list1[0] ++ merge(list1[1:], list2)
 *     else
 *         return list2[0] ++ merge(list1, list2[1:])
 * else if(N > 0)
 *     return list1
 * else
 *     return list2
 */
// TODO: Have a list(list, size_type, size_type) constructor (avoids 2 copies of list).
template<typename T, std::size_t N, std::size_t M, class Compare>
constexpr basic_list<T, N+M> merge(basic_list<T, N> list1, basic_list<T, M> list2, Compare cmp) {
	return N > 0 && M > 0
		? cmp(list1[0], list2[0])
			? basic_list<T, N+M>(merge(
				basic_list<T, (N > 0 ? N-1 : 0)>(list1.begin() + 1, list1.end()),
				list2,
				cmp).insert(0, list1[0])
			: basic_list<T, N+M>(merge(
				list1,
				basic_list<T, (M > 0 ? M-1 : 0)>(list2.begin() + 1, list2.end()),
				cmp).insert(0, list2[0])
		: N > 0
			? basic_list<T, N+M>(list1)  // HINT: N+M == N
			: basic_list<T, N+M>(list2); // HINT: N+M == M
}

template<typename T, std::size_t N, std::size_t M, class Compare>
constexpr basic_list<T, N+M> sort(basic_list<T, N> list1, basic_list<T, M> list2, Compare cmp) {
	return merge(sort(list1, cmp), sort(list2, cmp), cmp);
}

}

template<typename T, std::size_t N>
constexpr basic_list<T, N> sort(basic_list<T, N> const& list) {
	return sort(list, std::less_equal<T>());
}

// TODO: Is this stable? Decide whether cexpr::sort should guarantee stability.
template<typename T, std::size_t N, class Compare>
constexpr basic_list<T, N> sort(basic_list<T, N> const& list, Compare cmp) {
	return N == 1
		? list
		: sort(basic_list<T, N/2>(list.begin(), list.begin() + N/2),
		       basic_list<T, (N+1)/2>(list.begin() + N/2, list.end()),
		       cmp);
}

}

#endif
