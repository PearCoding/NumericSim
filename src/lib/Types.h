#pragma once

#include <type_traits>
#include <complex>

#include "nsConfig.h"

NS_BEGIN_NAMESPACE

/**
 * @defgroup TypeTraits Type Traits
 * std:: conform type traits to be used by templates.
 */

template<class T>
struct is_complex : std::integral_constant<bool,
	std::is_same<std::complex<float>, typename std::remove_cv<T>::type>::value ||
	std::is_same<std::complex<double>, typename std::remove_cv<T>::type>::value ||
	std::is_same<std::complex<long double>, typename std::remove_cv<T>::type>::value> {};

/**
* @brief A std:: conform type-trait for the numbers supported in NumericSim.
* @sa is_complex
* @ingroup TypeTraits
*/
template<class T>
struct is_number : std::integral_constant<bool,
	std::is_arithmetic<T>::value ||
	is_complex<T>::value> {};

/**
* @brief Returns the internal type of the complex
* @sa get_complex_internal<std::complex<T> >
* @ingroup TypeTraits
*/
template<typename T>
struct get_complex_internal
{
	typedef T type;
};

/**
* @brief The specialization of get_complex_internal
* @sa get_complex_internal<T>
* @ingroup TypeTraits
*/
template<typename T>
struct get_complex_internal<std::complex<T> > 
{
	typedef T type;
};

// Variable Template functions
template<typename T>
bool sqrt_allowed(const T& t)
{
	return t >= 0;
}

template<typename T>
bool sqrt_allowed(const std::complex<T>& t)
{
	return true;
}

template<typename T>
T complex_conj(const T& t)
{
	return t;
}

template<typename T>
std::complex<T> complex_conj(const std::complex<T>& t)
{
	return std::conj(t);
}

template<typename T>
T complex_abs(const T& t)
{
	return t;
}

template<typename T>
T complex_abs(const std::complex<T>& t)
{
	return std::abs(t);
}

template<typename T>
T complex_isnan(const T& t)
{
	return std::isnan(t);
}

template<typename T>
T complex_isnan(const std::complex<T>& t)
{
	return std::isnan(t.real()) || std::isnan(t.imag());
}

template<typename T>
T complex_isinf(const T& t)
{
	return std::isinf(t);
}

template<typename T>
T complex_isinf(const std::complex<T>& t)
{
	return std::isinf(t.real()) || std::isinf(t.imag());
}

NS_END_NAMESPACE