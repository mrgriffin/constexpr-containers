/*!
 * \file list.hpp
 * \brief \c constexpr statically-sized container.
 */

#ifndef CEXPR_DETAIL_LIST
#define CEXPR_DETAIL_LIST

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace cexpr {
namespace detail {

//! \c constexpr list containing exactly \p N elements of type \p T.
template<typename T, std::size_t N>
class list {
public:
	typedef T value_type;
	typedef std::size_t size_type;

	//! Constructs a list containing \p N elements equal to \c T().
	constexpr list();

	//! Constructs a list containing \p N elements equal to \p value.
	constexpr explicit list(T const& value);

	//! Constructs a list containing the first \p N elements of \p that padded with \c T() if necessary.
	template<std::size_t M>
	constexpr list(list<T, M> const& that);

	//! Constructs a list containing the first \p N elements of \p that padded with \p value if necessary.
	template<std::size_t M>
	constexpr list(list<T, M> const& that, T const& value);

	//! Constructs a list containing the first \p N elements of \p values padded with \c T() if necessary.
	template<std::size_t M>
	constexpr explicit list(T (&values)[M]);

	//! Constructs a list containing the first \p N elements of \p values padded with \p value if necessary.
	template<std::size_t M>
	constexpr list(T (&values)[M], T const& value);

	//! Constructs a list containing the first \p N elements of \p values padded with \c T() if necessary.
	constexpr explicit list(std::initializer_list<T> values);

	//! Constructs a list containing the first \p N elements of \p values padded with \p value if necessary.
	constexpr list(std::initializer_list<T> values, T const& value);

	//! Returns the element at \p pos.
	constexpr T const& operator[](size_type pos);

	//! Overwrites the element at \p pos.
	constexpr list set(size_type pos, T const& value);

	//! Inserts an element at \p pos.
	constexpr list insert(size_type pos, T const& value);

	//! Inserts a series of elements at \p pos.
	constexpr list insert(size_type pos, std::initializer_list<T> values);

	//! Removes the element at \p pos.
	constexpr list erase(size_type pos);

	//! Removes the elements in the range [ \p first, \p last ).
	constexpr list erase(size_type first, size_type last);

	//! Returns true if this list contains no elements.
	constexpr bool empty();

	//! Returns the number of elements.
	constexpr size_type size();

	//! Returns the maximum number of elements.
	constexpr size_type max_size();

private:
	T head;
	list<T, N-1> tail;

	size_type length;
};

template<typename T>
class list<T, 0> {
public:
	typedef T value_type;
	typedef std::size_t size_type;

	constexpr list() {}
	constexpr explicit list(T const& value) {}

	template<std::size_t M>
	constexpr explicit list(list<T, M> const& that) {}

	template<std::size_t M>
	constexpr list(list<T, M> const& that, T const& value) {}

	template<std::size_t M>
	constexpr explicit list(T (&values)[M]) {}

	template<std::size_t M>
	constexpr list(T (&values)[M], T const& value) {}

	constexpr explicit list(std::initializer_list<T> values) {}
	constexpr list(std::initializer_list<T> values, T const& value) {}

	constexpr T operator[](size_type pos) { return fail<true>(), T(); }

	constexpr list set(size_type pos, T const& value) { return fail<true>(), *this; }

	constexpr list insert(size_type pos, T const& value) { return fail<true>(), *this; }
	constexpr list insert(size_type pos, std::initializer_list<T> values) { return fail<true>(), *this; }

	constexpr list erase(size_type pos) { return fail<true>(), *this; }
	constexpr list erase(size_type first, size_type last) { return fail<true>(), *this; }

	constexpr bool empty() { return true; }
	constexpr size_type size() { return 0; }
	constexpr size_type max_size() { return 0; }

private:
	template<bool b>
	constexpr int fail() { static_assert(!b, "attempt to access outside of array"); return 0; }
};

}
}

#endif
