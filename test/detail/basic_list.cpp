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

//! A test that should fail at compile time.
// TODO: Create a unit test framework that ensures the FAILs fail.
#define FAIL(...)

//! The size and values in the list being tested.
//! \param T the type of the elements of the list.
//! \param size the maximum number of elements in the list.
//! \param ... the parameters to the constructor of the list.
#define WITH(T, size, ...) \
typedef T value_type; \
constexpr auto list_ = basic_list<T, size>(__VA_ARGS__)

//! The operation being tested.
//! \param ... the operation as it would appear after \c basic_list<T, N>().
#define DO(...) \
constexpr auto list = list_ __VA_ARGS__

//! The expected result of the operation.
//! \param ex_size the expected size of the list.
//! \param ... the expected values of the list as an initializer list.
#define EXPECTING(ex_size, ...) \
constexpr std::initializer_list<value_type> values = __VA_ARGS__; \
STATIC_ASSERT_EQUALS(list, values, "list != " #__VA_ARGS__); \
STATIC_ASSERT_EQUALS(list.size(), ex_size)

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

// basic_list<T, N>()
TEST(ctor_0) { WITH(int, 0,); DO(); EXPECTING(0, {}); }
TEST(ctor_1) { WITH(int, 1,); DO(); EXPECTING(1, {0}); }
TEST(ctor_n) { WITH(int, 2,); DO(); EXPECTING(2, {0,0}); }


// basic_list<T, N>(T const&)
TEST(ctor_T_0) { WITH(int, 0, 1); DO(); EXPECTING(0, {}); }
TEST(ctor_T_1) { WITH(int, 1, 1); DO(); EXPECTING(1, {1}); }
TEST(ctor_T_n) { WITH(int, 2, 1); DO(); EXPECTING(2, {1,1}); }


// basic_list<T, N>(basic_list<T, M> const&)
TEST(copy_0_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 0, that); DO(); EXPECTING(0, {}); }
TEST(copy_0_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 0, that); DO(); EXPECTING(0, {}); }
TEST(copy_0_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 0, that); DO(); EXPECTING(0, {}); }

TEST(copy_1_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 1, that); DO(); EXPECTING(1, {0}); }
TEST(copy_1_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 1, that); DO(); EXPECTING(1, {1}); }
TEST(copy_1_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 1, that); DO(); EXPECTING(1, {1}); }

TEST(copy_n_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 2, that); DO(); EXPECTING(2, {0,0}); }
TEST(copy_n_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 2, that); DO(); EXPECTING(2, {1,0}); }
TEST(copy_n_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 2, that); DO(); EXPECTING(2, {1,1}); }


// basic_list<T, N>(basic_list<T, M> const&, T const&)
TEST(copy_T_0_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 0, that, 2); DO(); EXPECTING(0, {}); }
TEST(copy_T_0_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 0, that, 2); DO(); EXPECTING(0, {}); }
TEST(copy_T_0_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 0, that, 2); DO(); EXPECTING(0, {}); }

TEST(copy_T_1_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 1, that, 2); DO(); EXPECTING(1, {2}); }
TEST(copy_T_1_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 1, that, 2); DO(); EXPECTING(1, {1}); }
TEST(copy_T_1_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 1, that, 2); DO(); EXPECTING(1, {1}); }

TEST(copy_T_n_0) { constexpr auto that = basic_list<int, 0>(1); WITH(int, 2, that, 2); DO(); EXPECTING(2, {2,2}); }
TEST(copy_T_n_1) { constexpr auto that = basic_list<int, 1>(1); WITH(int, 2, that, 2); DO(); EXPECTING(2, {1,2}); }
TEST(copy_T_n_n) { constexpr auto that = basic_list<int, 2>(1); WITH(int, 2, that, 2); DO(); EXPECTING(2, {1,1}); }


// basic_list<T, M>(T const (&values)[M])
TEST(ctor_Ts_1_0) { constexpr int that[1] = {1}; WITH(int, 0, that); DO(); EXPECTING(0, {}); }
TEST(ctor_Ts_1_1) { constexpr int that[1] = {1}; WITH(int, 1, that); DO(); EXPECTING(1, {1}); }
TEST(ctor_Ts_1_n) { constexpr int that[1] = {1}; WITH(int, 2, that); DO(); EXPECTING(2, {1,0}); }

TEST(ctor_Ts_n_0) { constexpr int that[2] = {1,2}; WITH(int, 0, that); DO(); EXPECTING(0, {}); }
TEST(ctor_Ts_n_1) { constexpr int that[2] = {1,2}; WITH(int, 1, that); DO(); EXPECTING(1, {1}); }
TEST(ctor_Ts_n_n) { constexpr int that[2] = {1,2}; WITH(int, 2, that); DO(); EXPECTING(2, {1,2}); }


// basic_list<T, M>(T const (&values)[M], T const&)
TEST(ctor_Ts_T_1_0) { constexpr int that[1] = {1}; WITH(int, 0, that, 3); DO(); EXPECTING(0, {}); }
TEST(ctor_Ts_T_1_1) { constexpr int that[1] = {1}; WITH(int, 1, that, 3); DO(); EXPECTING(1, {1}); }
TEST(ctor_Ts_T_1_n) { constexpr int that[1] = {1}; WITH(int, 2, that, 3); DO(); EXPECTING(2, {1,3}); }

TEST(ctor_Ts_T_n_0) { constexpr int that[2] = {1,2}; WITH(int, 0, that, 3); DO(); EXPECTING(0, {}); }
TEST(ctor_Ts_T_n_1) { constexpr int that[2] = {1,2}; WITH(int, 1, that, 3); DO(); EXPECTING(1, {1}); }
TEST(ctor_Ts_T_n_n) { constexpr int that[2] = {1,2}; WITH(int, 2, that, 3); DO(); EXPECTING(2, {1,2}); }


// basic_list<T, M>(std::initializer_list<T>)
TEST(ctor_ilist_0_0) { WITH(int, 0, {}); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_0_1) { WITH(int, 1, {}); DO(); EXPECTING(1, {0}); }
TEST(ctor_ilist_0_n) { WITH(int, 2, {}); DO(); EXPECTING(2, {0,0}); }

TEST(ctor_ilist_1_0) { WITH(int, 0, {1}); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_1_1) { WITH(int, 1, {1}); DO(); EXPECTING(1, {1}); }
TEST(ctor_ilist_1_n) { WITH(int, 2, {1}); DO(); EXPECTING(2, {1,0}); }

TEST(ctor_ilist_n_0) { WITH(int, 0, {1,2}); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_n_1) { WITH(int, 1, {1,2}); DO(); EXPECTING(1, {1}); }
TEST(ctor_ilist_n_n) { WITH(int, 2, {1,2}); DO(); EXPECTING(2, {1,2}); }


// basic_list<T, M>(std::initializer_list<T>, T const&)
TEST(ctor_ilist_T_0_0) { WITH(int, 0, {}, 3); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_T_0_1) { WITH(int, 1, {}, 3); DO(); EXPECTING(1, {3}); }
TEST(ctor_ilist_T_0_n) { WITH(int, 2, {}, 3); DO(); EXPECTING(2, {3,3}); }

TEST(ctor_ilist_T_1_0) { WITH(int, 0, {1}, 3); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_T_1_1) { WITH(int, 1, {1}, 3); DO(); EXPECTING(1, {1}); }
TEST(ctor_ilist_T_1_n) { WITH(int, 2, {1}, 3); DO(); EXPECTING(2, {1,3}); }

TEST(ctor_ilist_T_n_0) { WITH(int, 0, {1,2}, 3); DO(); EXPECTING(0, {}); }
TEST(ctor_ilist_T_n_1) { WITH(int, 1, {1,2}, 3); DO(); EXPECTING(1, {1}); }
TEST(ctor_ilist_T_n_n) { WITH(int, 2, {1,2}, 3); DO(); EXPECTING(2, {1,2}); }


// operator[](size_type)
FAIL(TEST(index_op_oob_0) { WITH(int, 0,); list[0]; })
FAIL(TEST(index_op_oob_1) { WITH(int, 1,); list[1]; })
FAIL(TEST(index_op_oob_2) { WITH(int, 2,); list[2]; })


// set(size_type, T const&)
FAIL(TEST(set_0_0) { WITH(int, 0, {}); DO(.set(0, 2)); })
TEST(set_0_1) { WITH(int, 1, {0}); DO(.set(0, 2)); EXPECTING(1, {2}); }
TEST(set_0_n) { WITH(int, 2, {0,1}); DO(.set(0, 2)); EXPECTING(2, {2,1}); }

FAIL(TEST(set_1_1) { WITH(int, 1, {0}); DO(.set(1, 2)); })
TEST(set_1_n) { WITH(int, 2, {0,1}); DO(.set(1, 2)); EXPECTING(2, {0,2}); }

FAIL(TEST(set_n_n) { WITH(int, 2, {0,1}); DO(.set(2, 2)); })


// insert(size_type, T const&)
FAIL(TEST(insert_T_0_0) { WITH(int, 0, {}); DO(.insert(0, 2)); } })
TEST(insert_T_0_1) { WITH(int, 1, {0}); DO(.insert(0, 2)); EXPECTING(1, {2}); }
TEST(insert_T_0_n) { WITH(int, 2, {0,1}); DO(.insert(0, 2)); EXPECTING(2, {2,0}); }

FAIL(TEST(insert_T_1_1) { WITH(int, 1, {0}); DO(.insert(1, 2)); })
TEST(insert_T_1_n) { WITH(int, 2, {0,1}); DO(.insert(1, 2)); EXPECTING(2, {0,2}); }

FAIL(TEST(insert_T_n_n) { WITH(int, 2, {0,1}); DO(.insert(2, 2)); })


// insert(size_type, std::initializer_list<T>)
FAIL(TEST(insert_Ts_0_0_0) { WITH(int, 0, {}); DO(.insert(0, {})); })
TEST(insert_Ts_0_0_1) { WITH(int, 1, {0}); DO(.insert(0, {})); EXPECTING(1, {0}); }
TEST(insert_Ts_0_0_n) { WITH(int, 2, {0,1}); DO(.insert(0, {})); EXPECTING(2, {0,1}); }

TEST(insert_Ts_0_1_1) { WITH(int, 1, {0}); DO(.insert(0, {2})); EXPECTING(1, {2}); }
TEST(insert_Ts_0_1_n) { WITH(int, 2, {0,1}); DO(.insert(0, {2})); EXPECTING(2, {2,0}); }

TEST(insert_Ts_0_2_1) { WITH(int, 1, {0}); DO(.insert(0, {2,3})); EXPECTING(1, {2}); }
TEST(insert_Ts_0_2_n) { WITH(int, 2, {0,1}); DO(.insert(0, {2,3})); EXPECTING(2, {2,3}); }

FAIL(TEST(insert_Ts_1_0_1) { WITH(int, 1, {0}); DO(.insert(1, {})); })
TEST(insert_Ts_1_0_n) { WITH(int, 2, {0,1}); DO(.insert(1, {})); EXPECTING(2, {0,1}); }

TEST(insert_Ts_1_1_n) { WITH(int, 2, {0,1}); DO(.insert(1, {2})); EXPECTING(2, {0,2}); }

TEST(insert_Ts_1_2_n) { WITH(int, 2, {0,1}); DO(.insert(1, {2,3})); EXPECTING(2, {0,2}); }

FAIL(TEST(insert_Ts_2_0_n) { WITH(int, 2, {0,1}); DO(.insert(2, {})); })

int main() {}
