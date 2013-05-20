/**
 * @file
 * @author    Bernd Amend <berndamend@gemail.com>
 * @date      2013
 * @copyright Copyright Bernd Amend 2013
 *            Distributed under the Boost Software License, Version 1.0.
 *            (See accompanying file LICENSE_1_0.txt or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include <functional>
#include <type_traits>
#include <string>
#include <tuple>
#include <initializer_list>

namespace pattern_matching {

// and, or
// Option/None

struct range_empty {};
struct range_one {};
struct range_all {};

struct range {
	// (-∞..+∞)
	constexpr static range_all all() { return range_all(); }

//	open(C, C) // (a..b)
//	closed(C, C) // [a..b]
//	closedOpen(C, C) // [a..b)
//	openClosed(C, C) // (a..b]
//	greaterThan(C) // (a..+∞)
//	atLeast(C) // [a..+∞)
//	lessThan(C) //(-∞..b)
//	atMost(C) // (-∞..b]
};

template<bool>
struct setvalue;

template<>
struct setvalue<true> {
	template<typename T1, typename T2>
	static void set(T1 e1, T2 e2) {
		*e1 = e2;
	}
};

template<>
struct setvalue<false> {
	template<typename T1, typename T2>
	static void set(T1 /*e1*/, T2 /*e2*/) {}
};

template<bool>
struct compare;

template<>
struct compare<true> {
	template<typename T1, typename T2>
	static bool c( T1 e1, T2 e2) {
		return e1 == e2;
	}
};

template<>
struct compare<false> {
	template<typename T1, typename T2>
	static bool c(T1 /*e1*/, T2 /*e2*/) {
		return false;
	}
};

template<typename T>
struct ref {

};

template<typename T>
struct has_type_const_iterator
{
private:
	typedef char                      yes;
	typedef struct { char array[2]; } no;

	template<typename C> static yes test(typename C::const_iterator*);
	template<typename C> static no  test(...);
public:
	static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template<typename T, typename Enable=void>
struct matcher;

template<typename T>
struct matcher<T,  typename std::enable_if<has_type_const_iterator<T>::value>::type> {

	template<typename T1>
	static bool m_helper(const T &m, typename T::const_iterator &iter, bool &last_element_was_dont_care, const T1 &element) {
		if (std::is_pointer<T1>::value) {
			setvalue<std::is_pointer<T1>::value>::set(element, *iter);
			iter++;
			last_element_was_dont_care = false;
		} else {
			if(std::is_same<T1, range_all>::value)
				last_element_was_dont_care = true;
			else {
				if(last_element_was_dont_care) {
					// advance to element or matcher with false
					bool found = false;
					for(;iter != m.end(); ++iter) {
						if (compare<!std::is_pointer<T1>::value && !std::is_same<T1, range_all>::value>::c(*iter, element)) {
							found = true;
							break;
						}
					}

					if(!found) {
						return false;
					}

				} else {
					if (!compare<!std::is_pointer<T1>::value && !std::is_same<T1,range_all>::value>::c(*iter, element))
						return false;
				}
				last_element_was_dont_care = false;
				iter++;
			}
		}
		return true;
	}

	template<typename T1, typename... TR>
	static bool m_impl(const T &m, typename T::const_iterator &iter, bool &last_element_was_dont_care, const T1 & head ) {

		if(!m_helper(m, iter, last_element_was_dont_care, head))
			return false;

		return true;
	}

	template<typename T1, typename... TR>
	static bool m_impl(const T &m, typename T::const_iterator &iter, bool &last_element_was_dont_care, const T1 & head, const TR &... tail ) {

		if(!m_helper(m, iter, last_element_was_dont_care, head))
			return false;
		if(!m_impl(m, iter, last_element_was_dont_care, tail...))
			return false;

		return true;
	}

	template<typename... TR>
	static bool m(const T &m, const TR &... tail ) {
		bool last_element_was_dont_care = false;
		typename T::const_iterator iter = m.begin();

		m_impl(m, iter, last_element_was_dont_care, tail...);

		return last_element_was_dont_care;
	}

};

struct Address {
	std::string firstname, lastname;
};

template<>
struct matcher<Address, std::enable_if<true>::type> {

	template<typename T1, typename T2>
	static bool m(const Address &e, const std::tuple<T1, T2> &a1) {

		if(std::get<0>(a1) == nullptr && std::get<1>(a1)  != e.firstname && std::get<1>(a1)  != e.lastname)
			return false;

		if(std::get<0>(a1) == &Address::firstname && std::get<1>(a1)  != e.firstname)
			return false;

		if(std::get<0>(a1) == &Address::lastname && std::get<1>(a1) != e.lastname)
			return false;

		return true;
	}
};

template<typename T, typename... TR>
bool match(const T &m, const TR &... tail) {
	return matcher<T>::m(m, tail...);
}

}
