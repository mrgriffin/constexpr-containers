/*!
 * \file sort_asic_list.cpp
 * \brief Demonstrates a merge sort on a basic_list.
 */

#include "cexpr/detail/basic_list.hpp"

using cexpr::detail::basic_list;

// TODO: Can we pass lists by const reference?
// TODO: Use size_type parameters instead of splitting the lists?
// TODO: Use a comparator.

template<typename T, size_t N, size_t M>
constexpr basic_list<T, N+M> merge(basic_list<T, N> list1, basic_list<T, M> list2);

template<typename T, size_t N, size_t M>
struct merger {
	constexpr basic_list<T, N+M> operator()(basic_list<T, N> list1, basic_list<T, M> list2) {
		return list1[0] <= list2[0]
			? basic_list<T, N+M>(merge(basic_list<T, N-1>(list1.begin() + 1, list1.end()), list2)).insert(0, list1[0])
			: basic_list<T, N+M>(merge(list1, basic_list<T, M-1>(list2.begin() + 1, list2.end()))).insert(0, list2[0]);
	}
};

template<typename T, size_t N>
struct merger<T, N, 0> {
	constexpr basic_list<T, N> operator()(basic_list<T, N> list1, basic_list<T, 0> list2) {
		return list1;
	}
};

template<typename T, size_t M>
struct merger<T, 0, M> {
	constexpr basic_list<T, M> operator()(basic_list<T, 0> list1, basic_list<T, M> list2) {
		return list2;
	}
};

template<typename T>
struct merger<T, 0, 0> {
	constexpr basic_list<T, 0> operator()(basic_list<T, 0> list1, basic_list<T, 0> list2) {
		return {};
	}
};

template<typename T, size_t N, size_t M>
constexpr basic_list<T, N+M> merge(basic_list<T, N> list1, basic_list<T, M> list2) {
	return merger<T, N, M>()(list1, list2);
}

template<typename T, size_t N>
constexpr basic_list<T, N> sort(basic_list<T, N> const& list);

template<typename T, size_t N, size_t M>
constexpr basic_list<T, N+M> sort(basic_list<T, N> list1, basic_list<T, M> list2) {
	return merge(sort(list1), sort(list2));
}

template<typename T, size_t N>
struct sorter {
	constexpr basic_list<T, N> operator()(basic_list<T, N> const& list) {
		return sort(basic_list<T, N/2>(list.begin(), list.begin() + N/2),
			    basic_list<T, (N+1)/2>(list.begin() + N/2, list.end()));
	}
};

template<typename T>
struct sorter<T, 1> {
	constexpr basic_list<T, 1> operator()(basic_list<T, 1> const& list) {
		return list;
	}
};

template<typename T, size_t N>
constexpr basic_list<T, N> sort(basic_list<T, N> const& list) {
	return sorter<T, N>()(list);
}

int main() {
	constexpr basic_list<int, 10> is({ 1, 5, 6, 3, 4, 2, 8, 0, 9, 7 });
	constexpr auto sorted_is = sort(is);
	static_assert(sorted_is[0] == 0, "[0]");
	static_assert(sorted_is[1] == 1, "[1]");
	static_assert(sorted_is[2] == 2, "[2]");
	static_assert(sorted_is[3] == 3, "[3]");
	static_assert(sorted_is[4] == 4, "[4]");
	static_assert(sorted_is[5] == 5, "[5]");
	static_assert(sorted_is[6] == 6, "[6]");
	static_assert(sorted_is[7] == 7, "[7]");
	static_assert(sorted_is[8] == 8, "[8]");
	static_assert(sorted_is[9] == 9, "[9]");
}
