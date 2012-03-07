/*!
 * \file basic_list.cpp
 * \brief Unit-Tests for cexpr::detail::basic_list.
 * \details Assumes operator[] and size() are functional.
 */

#include "../../cexpr/detail/basic_list.hpp"

#define STATIC_ASSERT_EQUALS(value, expected, ...) STATIC_ASSERT_EQUALS_(value, expected, ##__VA_ARGS__, #value " != " #expected)
#define STATIC_ASSERT_EQUALS_(value, expected, message, ...) static_assert(value == expected, message)

#define PASTE2(x, y) PASTE2_(x, y)
#define PASTE2_(x, y) x ## y

//! Defines a test function.
#define TEST(name) \
void PASTE2(test_, name)()

//! Defines a test function that should fail at compile time.
#define TEST_FAIL(name)

//! The size and values in the list being tested.
#define WITH(T, size, values) \
typedef T value_type; \
constexpr auto list_ = basic_list<T, size>(values);

//! The operation being tested.
#define DO(operation) \
constexpr auto list = list_ operation;

//! The expected result of the operation.
#define EXPECTING(ex_size, ex_values) \
constexpr std::initializer_list<value_type> values = ex_values; \
STATIC_ASSERT_EQUALS(list, values, "list != " #ex_values); \
STATIC_ASSERT_EQUALS(list.size(), ex_size);

using cexpr::detail::basic_list;

template<typename T, std::size_t N>
constexpr bool equals(basic_list<T, N> const& list, std::initializer_list<T> ilist, std::size_t i) {
	return i == N
		? true
		: list[i] == *(ilist.begin() + i);
}

// Compares a basic_list<T> to an std::initializer_list<T>.
template<typename T, std::size_t N>
constexpr bool operator==(basic_list<T, N> const& list, std::initializer_list<T> ilist) {
	return equals(list, ilist, 0);
}

/*
 * basic_list<T, N>()
 */

//! Nullary constructor with \p N = 0.
void test_ctor_0() {
	constexpr auto list = basic_list<int, 0>();
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Nullary constructor with \p N = 1.
void test_ctor_1() {
	constexpr auto list = basic_list<int, 1>();
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Nullary constructor with \p N = 2.
void test_ctor_n() {
	constexpr auto list = basic_list<int, 2>();
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, N>(T const&)
 */

//! Unary constructor with \p value = 1 and \p N = 0.
void test_ctor_T_0() {
	constexpr auto list = basic_list<int, 0>(1);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Unary constructor with \p value = 1 and \p N = 1.
void test_ctor_T_1() {
	constexpr auto list = basic_list<int, 1>(1);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Unary constructor with \p value = 1 and \p N = 2.
void test_ctor_T_n() {
	constexpr auto list = basic_list<int, 2>(1);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, N>(basic_list<T, M> const&)
 */

//! Copy-constructor with \p N = 0 and \p M = 0.
void test_copy_0_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p N = 0 and \p M = 1.
void test_copy_0_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p N = 0 and \p M = 2.
void test_copy_0_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p N = 1 and \p M = 0.
void test_copy_1_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p N = 1 and \p M = 1.
void test_copy_1_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p N = 1 and \p M = 2.
void test_copy_1_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p N = 2 and \p M = 0.
void test_copy_n_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Copy-constructor with \p N = 2 and \p M = 1.
void test_copy_n_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Copy-constructor with \p N = 2 and \p M = 2.
void test_copy_n_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, N>(basic_list<T, M> const&, T const&)
 */

//! Copy-constructor with \p value = 2, \p N = 0 and \p M = 0.
void test_copy_T_0_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 0>(that, 2);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p value = 2, \p N = 0 and \p M = 1.
void test_copy_T_0_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 0>(that, 2);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p value = 2, \p N = 0 and \p M = 2.
void test_copy_T_0_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 0>(that, 2);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Copy-constructor with \p value = 2, \p N = 1 and \p M = 0.
void test_copy_T_1_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 1>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p value = 2, \p N = 1 and \p M = 1.
void test_copy_T_1_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 1>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p value = 2, \p N = 1 and \p M = 2.
void test_copy_T_1_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 1>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Copy-constructor with \p value = 2, \p N = 2 and \p M = 0.
void test_copy_T_n_0() {
	constexpr auto that = basic_list<int, 0>(1);
	constexpr auto list = basic_list<int, 2>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Copy-constructor with \p value = 2, \p N = 2 and \p M = 1.
void test_copy_T_n_1() {
	constexpr auto that = basic_list<int, 1>(1);
	constexpr auto list = basic_list<int, 2>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Copy-constructor with \p value = 2, \p N = 2 and \p M = 2.
void test_copy_T_n_n() {
	constexpr auto that = basic_list<int, 2>(1);
	constexpr auto list = basic_list<int, 2>(that, 2);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, M>(T const (&values)[M])
 */

//! Array-constructor with \p values = {}, \p N = 0 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_0_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = {}, \p N = 1 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_1_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = {}, \p N = 2 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_n_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Array-constructor with \p values = { 1 }, \p N = 0 and \p M = 1.
void test_ctor_Ts_0_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = { 1 }, \p N = 1 and \p M = 1.
void test_ctor_Ts_1_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = { 1 }, \p N = 2 and \p M = 1.
void test_ctor_Ts_n_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Array-constructor with \p values = { 1, 2 }, \p N = 0 and \p M = 2.
void test_ctor_Ts_0_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 0>(that);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = { 1, 2 }, \p N = 1 and \p M = 2.
void test_ctor_Ts_1_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 1>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = { 1, 2 }, \p N = 2 and \p M = 2.
void test_ctor_Ts_n_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 2>(that);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, M>(T const (&values)[M], T const&)
 */

//! Array-constructor with \p values = {}, \p value = 3, \p N = 0 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_T_0_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 0>(that, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = {}, \p value = 3, \p N = 1 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_T_1_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 1>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 3);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = {}, \p value = 3, \p N = 2 and \p M = 0.
//! \bug Test is not legal C++ (zero-length array).
void test_ctor_Ts_T_n_0() {
	constexpr int that[0] = {};
	constexpr auto list = basic_list<int, 2>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 3);
	STATIC_ASSERT_EQUALS(list[1], 3);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Array-constructor with \p values = { 1 }, \p value = 3, \p N = 0 and \p M = 1.
void test_ctor_Ts_T_0_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 0>(that, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = { 1 }, \p value = 3, \p N = 1 and \p M = 1.
void test_ctor_Ts_T_1_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 1>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = { 1 }, \p value = 3, \p N = 2 and \p M = 1.
void test_ctor_Ts_T_n_1() {
	constexpr int that[1] = { 1 };
	constexpr auto list = basic_list<int, 2>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 3);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Array-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 0 and \p M = 2.
void test_ctor_Ts_T_0_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 0>(that, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Array-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 1 and \p M = 2.
void test_ctor_Ts_T_1_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 1>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Array-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 2 and \p M = 2.
void test_ctor_Ts_T_n_n() {
	constexpr int that[2] = { 1, 2 };
	constexpr auto list = basic_list<int, 2>(that, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, M>(std::initializer_list<T>)
 */

//! Initializer-list-constructor with \p values = {}, \p N = 0.
void test_ctor_ilist_0_0() {
	constexpr auto list = basic_list<int, 0>({});
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = {}, \p N = 1.
void test_ctor_ilist_1_0() {
	constexpr auto list = basic_list<int, 1>({});
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = {}, \p N = 2.
void test_ctor_ilist_n_0() {
	constexpr auto list = basic_list<int, 2>({});
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Initializer-list-constructor with \p values = { 1 }, \p N = 0.
void test_ctor_ilist_0_1() {
	constexpr auto list = basic_list<int, 0>({ 1 });
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = { 1 }, \p N = 1.
void test_ctor_ilist_1_1() {
	constexpr auto list = basic_list<int, 1>({ 1 });
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = { 1 }, \p N = 2.
void test_ctor_ilist_n_1() {
	constexpr auto list = basic_list<int, 2>({ 1 });
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p N = 0.
void test_ctor_ilist_0_n() {
	constexpr auto list = basic_list<int, 0>({ 1, 2 });
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p N = 1.
void test_ctor_ilist_1_n() {
	constexpr auto list = basic_list<int, 1>({ 1, 2 });
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p N = 2.
void test_ctor_ilist_n_n() {
	constexpr auto list = basic_list<int, 2>({ 1, 2 });
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * basic_list<T, M>(std::initializer_list<T>, T const&)
 */

//! Initializer-list-constructor with \p values = {}, \p value = 3, \p N = 0.
void test_ctor_ilist_T_0_0() {
	constexpr auto list = basic_list<int, 0>({}, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = {}, \p value = 3, \p N = 1.
void test_ctor_ilist_T_1_0() {
	constexpr auto list = basic_list<int, 1>({}, 3);
	STATIC_ASSERT_EQUALS(list[0], 3);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = {}, \p value = 3, \p N = 2.
void test_ctor_ilist_T_n_0() {
	constexpr auto list = basic_list<int, 2>({}, 3);
	STATIC_ASSERT_EQUALS(list[0], 3);
	STATIC_ASSERT_EQUALS(list[1], 3);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Initializer-list-constructor with \p values = { 1 }, \p value = 3, \p N = 0.
void test_ctor_ilist_T_0_1() {
	constexpr auto list = basic_list<int, 0>({ 1 }, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = { 1 }, \p value = 3, \p N = 1.
void test_ctor_ilist_T_1_1() {
	constexpr auto list = basic_list<int, 1>({ 1 }, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = { 1 }, \p value = 3, \p N = 2.
void test_ctor_ilist_T_n_1() {
	constexpr auto list = basic_list<int, 2>({ 1 }, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 3);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 0.
void test_ctor_ilist_T_0_n() {
	constexpr auto list = basic_list<int, 0>({ 1, 2 }, 3);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 1.
void test_ctor_ilist_T_1_n() {
	constexpr auto list = basic_list<int, 1>({ 1, 2 }, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Initializer-list-constructor with \p values = { 1, 2 }, \p value = 3, \p N = 2.
void test_ctor_ilist_T_n_n() {
	constexpr auto list = basic_list<int, 2>({ 1, 2 }, 3);
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * operator[](size_type)
 */

//! Out-of-bounds index operation with \p N = 0.
void test_index_op_oob_0() { FAIL(
	constexpr auto list = basic_list<int, 0>();
	list[0];
)}

//! Out-of-bounds index operation with \p N = 1.
void test_index_op_oob_1() { FAIL(
	constexpr auto list = basic_list<int, 1>();
	list[1];
)}

//! Out-of-bounds index operation with \p N = 2.
void test_index_op_oob_n() { FAIL(
	constexpr auto list = basic_list<int, 2>();
	list[2];
)}

/*
 * set(size_type, T const&)
 */

//! Set with \p list = {}, \p pos = 0, \p value = 2.
void test_set_0_0() {
	constexpr auto list = basic_list<int, 0>().set(0, 2);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Set with \p list = { 0 }, \p pos = 0, \p value = 2.
void test_set_0_1() {
	constexpr auto list = basic_list<int, 1>({ 0 }).set(0, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Set with \p list = { 0, 1 }, \p pos = 0, \p value = 2.
void test_set_0_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).set(0, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Set with \p list = { 0 }, \p pos = 1, \p value = 2.
void test_set_1_1() {
	constexpr auto list = basic_list<int, 1>({ 0 }).set(1, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Set with \p list = { 0, 1 }, \p pos = 1, \p value = 2.
void test_set_1_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).set(1, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Set with \p list = { 0, 1 }, \p pos = 2, \p value = 2.
void test_set_n_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).set(2, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * insert(size_type, T const&)
 */

//! Insert with \p list = {}, \p pos = 0, \p value = 2.
void test_insert_0_T_0() {
	constexpr auto list = basic_list<int, 0>().insert(0, 2);
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Insert with \p list = { 0 }, \p pos = 0, \p value = 2.
void test_insert_0_T_1() {
	constexpr auto list = basic_list<int, 1>({ 0 }).insert(0, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Insert with \p list = { 0, 1 }, \p pos = 0, \p value = 2.
void test_insert_0_T_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).insert(0, 2);
	STATIC_ASSERT_EQUALS(list[0], 2);
	STATIC_ASSERT_EQUALS(list[1], 0);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Insert with \p list = { 0 }, \p pos = 1, \p value = 2.
void test_insert_1_T_1() {
	constexpr auto list = basic_list<int, 1>({ 0 }).insert(1, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Insert with \p list = { 0, 1 }, \p pos = 1, \p value = 2.
void test_insert_1_T_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).insert(1, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Insert with \p list = { 0, 1 }, \p pos = 2, \p value = 2.
void test_insert_n_T_n() {
	constexpr auto list = basic_list<int, 2>({ 0, 1 }).insert(2, 2);
	STATIC_ASSERT_EQUALS(list[0], 0);
	STATIC_ASSERT_EQUALS(list[1], 1);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

/*
 * insert(size_type, std::initializer_list<T>)
 */

//! Insert with \p list = {}, \p pos = 0, \p values = {}
void test_insert_0_0_0() {
	constexpr auto list = basic_list<int, 0>({}).insert(0, {});
	STATIC_ASSERT_EQUALS(list.size(), 0);
}

//! Insert with \p list = { 1 }, \p pos = 0, \p values = {}
void test_insert_0_0_1() {
	constexpr auto list = basic_list<int, 1>({ 1 }).insert(0, {});
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list.size(), 1);
}

//! Insert with \p list = { 1, 2 }, \p pos = 0, \p values = {}
void test_insert_0_0_n() {
	constexpr auto list = basic_list<int, 2>({ 1, 2 }).insert(0, {});
	STATIC_ASSERT_EQUALS(list[0], 1);
	STATIC_ASSERT_EQUALS(list[1], 2);
	STATIC_ASSERT_EQUALS(list.size(), 2);
}

//! Insert with \p list = { 1 }, \p pos = 1, \p values = {}
void test_insert_0_1_1() {
	constexpr auto list = basic_list<int, 1>({ 1 }).insert(1, {});
}

int main() {
	test_ctor_0();           test_ctor_1();           test_ctor_n();

	test_ctor_T_0();         test_ctor_T_1();         test_ctor_T_n();

	test_copy_0_0();         test_copy_0_1();         test_copy_0_n();
	test_copy_1_0();         test_copy_1_1();         test_copy_1_n();
	test_copy_n_0();         test_copy_n_1();         test_copy_n_n();

	test_copy_T_0_0();       test_copy_T_0_1();       test_copy_T_0_n();
	test_copy_T_1_0();       test_copy_T_1_1();       test_copy_T_1_n();
	test_copy_T_n_0();       test_copy_T_n_1();       test_copy_T_n_n();

	test_ctor_Ts_0_0();      test_ctor_Ts_0_1();      test_ctor_Ts_0_n();
	test_ctor_Ts_1_0();      test_ctor_Ts_1_1();      test_ctor_Ts_1_n();
	test_ctor_Ts_n_0();      test_ctor_Ts_n_1();      test_ctor_Ts_n_n();

	test_ctor_Ts_T_0_0();    test_ctor_Ts_T_0_1();    test_ctor_Ts_T_0_n();
	test_ctor_Ts_T_1_0();    test_ctor_Ts_T_1_1();    test_ctor_Ts_T_1_n();
	test_ctor_Ts_T_n_0();    test_ctor_Ts_T_n_1();    test_ctor_Ts_T_n_n();

	test_ctor_ilist_0_0();   test_ctor_ilist_0_1();   test_ctor_ilist_0_n();
	test_ctor_ilist_1_0();   test_ctor_ilist_1_1();   test_ctor_ilist_1_n();
	test_ctor_ilist_n_0();   test_ctor_ilist_n_1();   test_ctor_ilist_n_n();

	test_ctor_ilist_T_0_0(); test_ctor_ilist_T_0_1(); test_ctor_ilist_T_0_n();
	test_ctor_ilist_T_1_0(); test_ctor_ilist_T_1_1(); test_ctor_ilist_T_1_n();
	test_ctor_ilist_T_n_0(); test_ctor_ilist_T_n_1(); test_ctor_ilist_T_n_n();

	test_set_0_0();          test_set_0_1();          test_set_0_n();
	                         test_set_1_1();          test_set_1_n();
	                                                  test_set_n_n();

	test_insert_0_T_0();     test_insert_0_T_1();     test_insert_0_T_n();
	                         test_insert_1_T_1();     test_insert_1_T_n();
	                                                  test_insert_n_T_n();

	test_insert_0_0_0(),     test_insert_0_0_1();     test_insert_0_0_n();
}
