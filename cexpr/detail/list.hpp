/*!
 * \file list.hpp
 * \brief \c constexpr statically-sized container.
 */

#ifndef CEXPR_DETAIL_LIST
#define CEXPR_DETAIL_LIST

#include <cstddef>
#include <initializer_list>

namespace cexpr {
namespace detail {

constexpr struct set_t { constexpr set_t() {} } SET;
constexpr struct insert_t { constexpr insert_t() {} } INSERT;

//! \c constexpr list containing exactly \p N elements of type \p T.
template<typename T, std::size_t N>
class basic_list {
friend class basic_list<T, N+1>;
friend class basic_list<T, N-1>;

public:
	typedef T value_type;
	typedef std::size_t size_type;

	//! Constructs a list containing up to \p N elements.
	//! \details The unused elements are default-constructed.
	constexpr basic_list()
		: head()
		{}

	//! Constructs a list containing up to \p N elements.
	//! \details The unused elements are copy-constructed from \p value.
	constexpr explicit basic_list(T const& value)
		: head(value)
		, tail(value)
		{}

	//! Constructs a list containing up to the first \p N elements of \p that.
	//! \details The unused elements are default-constructed.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that);

	//! Constructs a list containing up to the first \p N elements of \p that.
	//! \details The unused elements are copy-constructed from \p value.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, T const& value);

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The unused elements are default-constructed.
	template<std::size_t M>
	constexpr explicit basic_list(T const (&values)[M])
		: basic_list(values, M, T()) {}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The unused elements are copy-constructed from \p value.
	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], T const& value)
		: basic_list(values, M, value) {}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The unused elements are default-constructed.
	constexpr basic_list(std::initializer_list<T> values)
		: basic_list(values.begin(), values.end(), T()) {}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The unused elements are copy-constructed from \p value.
	constexpr basic_list(std::initializer_list<T> values, T const& value)
		: basic_list(values.begin(), values.end(), value) {}

	//! Returns the element at \p pos.
	constexpr T operator[](size_type pos) {
		return pos == 0
			? head
			: tail[pos - 1];
	}

	//! Overwrites the element at \p pos.
	constexpr basic_list set(size_type pos, T const& value) {
		return { *this, 0, pos, value, SET };
	}

	//! Inserts an element at \p pos.
	constexpr basic_list insert(size_type pos, T const& value) {
		return { *this, 0, pos, value, INSERT };
	}

	//! Inserts a series of elements at \p pos.
	constexpr basic_list insert(size_type pos, std::initializer_list<T> values);

	//! Removes the element at \p pos.
	constexpr basic_list erase(size_type pos) {
		return { *this, 0, pos };
	}

	//! Removes the elements in the range [ \p first, \p last ).
	constexpr basic_list erase(size_type first, size_type last);

	//! Returns the number of elements.
	constexpr size_type size() {
		return N;
	}

private:
	//! Constructs a list containing up to the first \p N elements in the range ( \p first, \p last ].
	//! \details The unused elements are copy-constructed from \p value.
	template<class InputIterator>
	constexpr basic_list(InputIterator first, InputIterator last, T const& value)
		: head(first != last ? *first : value)
		, tail(first + 1, last, value)
		{}

	//! Constructs a copy of \p that where the value at \p pos is equal to \p value.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos, T const& value, set_t)
		: head(count != pos ? that[count] : value)
		, tail(that, count + 1, pos, value, SET)
		{}

	//! Constructs a copy of \p that where the value at \p pos is equal to \p value and ( \p pos, \p N ] are as in \p that ( \p pos + 1, \p N ].
	//! \warning \p M must be >= \p N.
	//! \warning \p count must be < \c (size_type)-1.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos, T const& value, insert_t)
		: head(count != pos ? that[count] : value)
		, tail(that, count != pos ? count + 1 : count, count != pos ? pos : -1, value, INSERT)
		{}

	//! Constructs a copy of \p that where the values at ( \p pos, \p N ] are as in \p that ( \p pos + 1, \p N ].
	//! \details The value at \p N-1 is equal to that at \p N-2.
	//! \warning \p M must be >= \p N.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos)
		: head(count >= M ? that[M - 1] : count != pos ? that[count] : that[count + 1])
		, tail(that, count != pos ? count + 1 : count + 2, pos)
		{}

	T head;
	basic_list<T, N-1> tail;
};

template<typename T>
class basic_list<T, 0> {
friend class basic_list<T, 1>;

public:
	typedef T value_type;
	typedef std::size_t size_type;

	constexpr basic_list() {}
	constexpr explicit basic_list(T const& value) {}

	template<std::size_t M>
	constexpr explicit basic_list(basic_list<T, M> const& that) {}

	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, T const& value) {}

	template<std::size_t M>
	constexpr explicit basic_list(T const (&values)[M]) {}

	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], T const& value) {}

	constexpr basic_list(std::initializer_list<T> values) {}
	constexpr basic_list(std::initializer_list<T> values, T const& value) {}

	constexpr T operator[](size_type pos) { return fail(), T(); }

	constexpr basic_list set(size_type pos, T const& value) { return fail(), *this; }

	constexpr basic_list insert(size_type pos, T const& value) { return fail(), *this; }
	constexpr basic_list insert(size_type pos, std::initializer_list<T> values) { return fail(), *this; }

	constexpr basic_list erase(size_type pos) { return fail(), *this; }
	constexpr basic_list erase(size_type first, size_type last) { return fail(), *this; }

	constexpr size_type size() { return 0; }

private:
	constexpr int fail() { return throw "attempt to access outside of array", 0; }

	template<class InputIterator>
	constexpr basic_list(InputIterator first, InputIterator last, T const& value) {}

	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos, T const& value, set_t) {}

	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos, T const& value, insert_t) {}

	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, size_type count, size_type pos) {}
};

}
}

#endif
