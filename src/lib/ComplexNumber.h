#pragma once

#include "Config.h"

#include <initializer_list>
#include <type_traits>
#include <complex>

NS_BEGIN_NAMESPACE

/** A class representing a complex number
 */
template<typename T>
class ComplexNumber
{
public:
	ComplexNumber();
	ComplexNumber(T real);
	ComplexNumber(T real, T imag);
	ComplexNumber(std::initializer_list<T> l);

	ComplexNumber& operator=(T f);
	ComplexNumber& operator +=(const ComplexNumber& v2);
	ComplexNumber& operator +=(const T& f);
	ComplexNumber& operator -=(const ComplexNumber& v2);
	ComplexNumber& operator -=(const T& f);
	ComplexNumber& operator *=(const ComplexNumber& v2);
	ComplexNumber& operator *=(const T& f);
	ComplexNumber& operator /=(const ComplexNumber& v2);
	ComplexNumber& operator /=(const T& f);

	T real() const;
	T imag() const;

	ComplexNumber conjugate() const;
	ComplexNumber reciprocal() const;

	T abs() const;
	T mag() const;
	T magSqr() const;

	bool hasNaN() const;
	bool hasInf() const;
	bool isComplex() const;

private:
	T mReal;
	T mImag;
};

template<typename T>
ComplexNumber<T> operator +(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
ComplexNumber<T> operator +(const ComplexNumber<T>& v1, T f);
template<typename T>
ComplexNumber<T> operator +(T f, const ComplexNumber<T>& v1);
template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v1, T f);
template<typename T>
ComplexNumber<T> operator -(T f, const ComplexNumber<T>& v1);
template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v);
template<typename T>
ComplexNumber<T> operator *(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
ComplexNumber<T> operator *(const ComplexNumber<T>& v1, T f);
template<typename T>
ComplexNumber<T> operator *(T f, const ComplexNumber<T>& v1);
template<typename T>
ComplexNumber<T> operator /(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
ComplexNumber<T> operator /(const ComplexNumber<T>& v1, T f);
template<typename T>
ComplexNumber<T> operator /(T f, const ComplexNumber<T>& v1);

// Comparison
template<typename T>
bool operator ==(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
bool operator !=(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2);
template<typename T>
bool operator ==(const ComplexNumber<T>& v1, const T& v2);
template<typename T>
bool operator !=(const ComplexNumber<T>& v1, const T& v2);
template<typename T>
bool operator ==(const T& v1, const ComplexNumber<T>& v2);
template<typename T>
bool operator !=(const T& v1, const ComplexNumber<T>& v2);

/**
* @brief A std:: conform type-trait for the several complex number formats supported in NumericSim.
* @sa ComplexNumber
* @ingroup TypeTraits
*/
// Should be in Types.h, but we need it in other types implementation.
template<class T>
struct is_complex : std::integral_constant<bool,
	std::is_same<ComplexNumber<float>, typename std::remove_cv<T>::type>::value ||
	std::is_same<ComplexNumber<double>, typename std::remove_cv<T>::type>::value ||
	std::is_same<ComplexNumber<long double>, typename std::remove_cv<T>::type>::value> {};

/**
* @brief A std:: conform type-trait for the numbers supported in NumericSim.
* @sa is_complex
* @ingroup TypeTraits
*/
template<class T>
struct is_number : std::integral_constant<bool,
	std::is_floating_point<T>::value ||
	is_complex<T>::value> {};

// Variable Template functions
template<typename T>
T conjugate_vt(const T& t)
{
	return t;
}

template<typename T>
ComplexNumber<T> conjugate_vt(const ComplexNumber<T>& t)
{
	return t.conjugate();
}

NS_END_NAMESPACE

// Standard functions
namespace std
{
	template<typename T>
	T abs(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> exp(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> log(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> log10(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> pow(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> sqrt(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> sin(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> cos(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> tan(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> asin(const NS::ComplexNumber<T>& v);
	
	template<typename T>
	NS::ComplexNumber<T> acos(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> atan(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> sinh(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> cosh(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> tanh(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> asinh(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> acosh(const NS::ComplexNumber<T>& v);

	template<typename T>
	NS::ComplexNumber<T> atanh(const NS::ComplexNumber<T>& v);
}

#define _NS_COMPLEXNUMBER_INL
# include "ComplexNumber.inl"
#undef _NS_COMPLEXNUMBER_INL