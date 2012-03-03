/*!
 * \file list.hpp
 * \brief \c constexpr statically-sized container.
 */

#ifndef CEXPR_DETAIL_LIST
#define CEXPR_DETAIL_LIST

#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace cexpr {
namespace detail {

template<typename T, std::size_t N>
class basic_list;

template<typename T, std::size_t N>
class basic_list_iterator;

template<typename T, std::size_t N>
constexpr bool operator==(basic_list_iterator<T, N> lhs, basic_list_iterator<T, N> rhs);

template<typename T, std::size_t N>
constexpr bool operator!=(basic_list_iterator<T, N> lhs, basic_list_iterator<T, N> rhs);

//! \c constexpr \c std::iterator -like object for iterating through basic_list.
template<typename T, std::size_t N>
class basic_list_iterator {
public:
	constexpr basic_list_iterator(basic_list<T, N> const& list)
		: list(list)
		, position(0)
		{}

	constexpr basic_list_iterator(basic_list<T, N> const& list, typename basic_list<T, N>::size_type position)
		: list(list)
		, position(position)
		{}

	//! Advances forwards \p pos elements.
	constexpr basic_list_iterator operator+(typename basic_list<T, N>::size_type pos) {
		return { list, position + pos };
	}

	//! Moves backwards \p pos elements.
	constexpr basic_list_iterator operator-(typename basic_list<T, N>::size_type pos) {
		return { list, position - pos };
	}

	friend bool operator==<>(basic_list_iterator, basic_list_iterator);
	friend bool operator!=<>(basic_list_iterator, basic_list_iterator);

	//! Returns the current value.
	//! \warning Needs optimization.
	constexpr T operator*() {
		return list[position];
	}

private:
	basic_list<T, N> const& list;
	typename basic_list<T, N>::size_type position;
};

template<typename T, std::size_t N>
constexpr bool operator==(basic_list_iterator<T, N> lhs, basic_list_iterator<T, N> rhs) {
	// TODO: How do we compare two lists for identity equality?
	return /*&lhs.list == &rhs.list && */lhs.position == rhs.position;
}

template<typename T, std::size_t N>
constexpr bool operator!=(basic_list_iterator<T, N> lhs, basic_list_iterator<T, N> rhs) {
	return !(lhs == rhs);
}

//! \c constexpr list containing exactly \p N elements of type \p T.
template<typename T, std::size_t N>
class basic_list {
friend class basic_list<T, N+1>;
friend class basic_list<T, N-1>;
friend class basic_list_iterator<T, N>;

public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef basic_list_iterator<T, N> const_iterator;

	//! Constructs a list containing up to \p N elements.
	//! \details The elements are default-constructed.
	constexpr basic_list()
		: head()
		{}

	//! Constructs a list containing up to \p N elements.
	//! \details The elements are copy-constructed from \p value.
	constexpr explicit basic_list(T const& value)
		: head(value)
		, tail(value)
		{}

	//! Constructs a list containing up to the first \p N elements of \p that.
	//! \details The remaining elements are default-constructed.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that)
		: basic_list(that, T())
		{}

	constexpr basic_list(basic_list<T, 0> const& that)
		: basic_list()
		{}

	//! Constructs a list containing up to the first \p N elements of \p that.
	//! \details The remaining elements are copy-constructed from \p value.
	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, T const& value)
		: basic_list(that.cbegin(), that.cend(), value)
		{}

	constexpr basic_list(basic_list<T, 0> const& that, T const& value)
		: basic_list(value)
		{}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The remaining elements are default-constructed.
	template<std::size_t M>
	constexpr explicit basic_list(T const (&values)[M])
		: basic_list(values, T())
		{}

	constexpr explicit basic_list(T const (&values)[0])
		: basic_list()
		{}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The remaining elements are copy-constructed from \p value.
	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], T const& value)
		: basic_list(values, 0, value) {}

	constexpr explicit basic_list(T const (&values)[0], T const& value)
		: basic_list(value)
		{}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The remaining elements are default-constructed.
	constexpr basic_list(std::initializer_list<T> values)
		: basic_list(values, T()) {}

	//! Constructs a list containing up to the first \p N elements of \p values.
	//! \details The remaining elements are copy-constructed from \p value.
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
		return { cbegin(), cbegin() + pos, value, false, cbegin() + pos + 1, cend(), T() };
	}

	//! Inserts an element at \p pos.
	constexpr basic_list insert(size_type pos, T const& value) {
		// HINT: head will never be used.
		return { cbegin(), cbegin() + pos, value, false, cbegin() + pos, cend(), head };
	}

	//! Inserts a series of elements at \p pos.
	constexpr basic_list insert(size_type pos, std::initializer_list<T> values) {
		// HINT: head will never be used.
		return { cbegin(), cbegin() + pos, values.begin(), values.end(), cbegin() + pos, cend(), head };
	}

	//! Removes the element at \p pos.
	//! \details The element at \p N - 1 is default-constructed.
	constexpr basic_list erase(size_type pos) {
		return erase(pos, T());
	}

	//! Removes the element at \p pos.
	//! \details The element at \p N - 1 is copy-constructed from \p value.
	constexpr basic_list erase(size_type pos, T const& value) {
		return erase(pos, pos + 1, value);
	}

	//! Removes the elements in the range [ \p first, \p last ).
	//! \details The new elements at the end are default-constructed.
	constexpr basic_list erase(size_type first, size_type last) {
		return erase(first, last, T());
	}

	//! Removes the elements in the range [ \p first, \p last ).
	//! \details The new elements at the end are copy-constructed from \p value.
	constexpr basic_list erase(size_type first, size_type last, T const& value) {
		return { cbegin(), cbegin() + first, cbegin() + last, cend(), value };
	}

	//! Returns the number of elements.
	constexpr size_type size() {
		return N;
	}

	//! Returns an iterator to the first element.
	constexpr const_iterator begin() {
		return cbegin();
	}

	//! Returns an iterator to the first element.
	constexpr const_iterator cbegin() {
		return const_iterator(*this);
	}

	//! Returns an iterator to one-past the last element.
	constexpr const_iterator end() {
		return cend();
	}

	//! Returns an iterator to one-past the last element.
	constexpr const_iterator cend() {
		return const_iterator(*this, N);
	}

private:
	//! Constructs a list containing up to the first \p N elements in the range ( \p first, \p last ].
	//! \details The remaining elements are copy-constructed from \p value.
	template<class InputIterator>
	constexpr basic_list(InputIterator first, InputIterator last, T const& value)
		: head(first != last ? *first : value)
		, tail(first != last ? first + 1 : first, last, value)
		{}

	//! Constructs a list containing up to the first \p N elements in the range ( \p first1, \p last1 ] ++ ( \p first2, \p last2 ].
	//! \details The remaining elements are copy-constructed from \p value.
	template<class InputIterator1, class InputIterator2>
	constexpr basic_list(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		T const& value)
		: head(first1 != last1 ? *first1 : first2 != last2 ? *first2 : value)
		, tail(
			first1 != last1 ? first1 + 1 : first1, last1,
			first1 == last1 && first2 != last2 ? first2 + 1 : first2, last2,
			value)
		{}

	//! Constructs a list containing up to the first \p N elements in the range ( \p first1, \p last1 ] ++ \p insert ++ ( \p first2, \p last2 ].
	//! \details \p inserted should be false when called non-recursively.
	//! \details The remaining elements are copy-constructed from \p value.
	//! \deprecated Will be removed once I find a way to take an "iterator" to a single value.
	template<class InputIterator1, class InputIterator2>
	constexpr basic_list(
		InputIterator1 first1, InputIterator1 last1,
		T const& insert, bool inserted,
		InputIterator2 first2, InputIterator2 last2,
		T const& value)
		: head(first1 != last1 ? *first1 : !inserted ? insert : first2 != last2 ? *first2 : value)
		, tail(
			first1 != last1 ? first1 + 1 : first1, last1,
			insert, first1 == last1,
			first1 == last1 && inserted && first2 != last2 ? first2 + 1 : first2, last2,
			value)
		{}

	//! Constructs a list containing up to the first \p N elements in the range ( \p first1, \p last1 ] ++ ( \p first2, \p last2 ] ++ ( \p first3, \p last3 ].
	//! \details The remaining elements are copy-constructed from \p value.
	template<class InputIterator1, class InputIterator2, class InputIterator3>
	constexpr basic_list(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		InputIterator3 first3, InputIterator3 last3,
		T const& value)
		: head(first1 != last1 ? *first1 : first2 != last2 ? *first2 : first3 != last3 ? *first3 : value)
		, tail(
			first1 != last1 ? first1 + 1 : first1, last1,
			first1 == last1 && first2 != last2 ? first2 + 1 : first2, last2,
			first1 == last1 && first2 == last2 && first3 != last3 ? first3 + 1 : first3, last3,
			value)
		{}

	//! Constructs a list containing the first \p N elements in \p values.
	//! \details The remaining elements are copy-constructed from \p value.
	//! \deprecated Will be removed once GCC stops ICEing on std::begin.
	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], size_type pos, T const& value)
		: head(pos < M ? values[pos] : value)
		, tail(values, pos + 1, value)
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
	typedef basic_list_iterator<T, 0> const_iterator;

	constexpr basic_list() {}
	constexpr explicit basic_list(T const& value) {}

	template<std::size_t M>
	constexpr explicit basic_list(basic_list<T, M> const& that) {}

	template<std::size_t M>
	constexpr basic_list(basic_list<T, M> const& that, T const& value) {}

	template<std::size_t M>
	constexpr explicit basic_list(T const (&values)[M]) {}
	constexpr explicit basic_list(T const (&values)[0]) {}

	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], T const& value) {}
	constexpr basic_list(T const (&values)[0], T const& value) {}

	constexpr basic_list(std::initializer_list<T> values) {}
	constexpr basic_list(std::initializer_list<T> values, T const& value) {}

	constexpr T operator[](size_type pos) { return fail(), T(); }

	constexpr basic_list set(size_type pos, T const& value) { return *this; }

	constexpr basic_list insert(size_type pos, T const& value) { return *this; }
	constexpr basic_list insert(size_type pos, std::initializer_list<T> values) { return *this; }

	constexpr basic_list erase(size_type pos) { return *this; }
	constexpr basic_list erase(size_type first, size_type last) { return *this; }

	constexpr size_type size() { return 0; }

	// TODO: Fix cbegin and cend.
	//constexpr const_iterator begin() { return cbegin(); }
	//constexpr const_iterator cbegin() { return const_iterator(*this); }

	//constexpr const_iterator end() { return cend(); }
	//constexpr const_iterator cend() { return const_iterator(*this); }

private:
	constexpr int fail() { return throw "attempt to access outside of array", 0; }

	template<class InputIterator>
	constexpr basic_list(InputIterator first, InputIterator last, T const& value) {}

	template<class InputIterator1, class InputIterator2>
	constexpr basic_list(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, T const& value) {}
	
	template<class InputIterator1, class InputIterator2>
	constexpr basic_list(InputIterator1 first1, InputIterator1 last1, T const& insert, bool inserted, InputIterator2 first2, InputIterator2 last2, T const& value) {}

	template<class InputIterator1, class InputIterator2, class InputIterator3>
	constexpr basic_list(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, InputIterator3 first3, InputIterator3 last3, T const& value) {}

	template<std::size_t M>
	constexpr basic_list(T const (&values)[M], size_type pos, T const& value) {}
};

}
}

#endif
