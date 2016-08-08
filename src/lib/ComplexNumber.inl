#ifndef _NS_COMPLEXNUMBER_INL
# error ComplexNumber.inl should only be included by ComplexNumber.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
ComplexNumber<T>::ComplexNumber() :
	ComplexNumber(0, 0)
{
}

template<typename T>
ComplexNumber<T>::ComplexNumber(T real) :
	ComplexNumber(real, 0)
{
}

template<typename T>
ComplexNumber<T>::ComplexNumber(T real, T imag) :
	mReal(real), mImag(imag)
{
	static_assert(std::is_floating_point<T>::value, "Type T has to be of floating point type.\nSee C++ reference of std::is_floating_point for more information.");
}

template<typename T>
ComplexNumber<T>::ComplexNumber(std::initializer_list<T> l) :
	ComplexNumber()
{
	NS_ASSERT(l.size() <= 2);// Would be great if we could use size() with constexpr [C++14]

	auto it = l.begin();
	if (it != l.end())
	{
		mReal = *it;
		++it;
		if (it != l.end())
			mImag = *it;
	}
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator=(T f)
{
	mReal = f;
	mImag = 0;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator +=(const ComplexNumber<T>& v2)
{
	mReal += v2.real();
	mImag += v2.imag();

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator +=(const T& f)
{
	mReal += f;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator -=(const ComplexNumber& v2)
{
	mReal -= v2.real();
	mImag -= v2.imag();

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator -=(const T& f)
{
	mReal -= f;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator *=(const ComplexNumber& v2)
{
	T r = mReal*v2.real() - mImag*v2.imag();
	mImag = mImag*v2.real() + mReal*v2.imag();
	mReal = r;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator *=(const T& f)
{
	mReal *= f;
	mImag *= f;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator /=(const ComplexNumber& v2)
{
	auto n = v2.magSqr();

	auto r = mReal*v2.real() + mImag*v2.imag();
	mImag = (mImag*v2.real() - mReal*v2.imag())/n;
	mReal = r/n;

	return *this;
}

template<typename T>
ComplexNumber<T>& ComplexNumber<T>::operator /=(const T& f)
{
	mReal /= f;
	mImag /= f;
	return *this;
}

template<typename T>
T ComplexNumber<T>::real() const
{
	return mReal;
}

template<typename T>
T ComplexNumber<T>::imag() const
{
	return mImag;
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::conjugate() const
{
	return ComplexNumber<T>(mReal, -mImag);
}

template<typename T>
ComplexNumber<T> ComplexNumber<T>::reciprocal() const
{
	return (conjugate() /= magSqr());
}

template<typename T>
T ComplexNumber<T>::abs() const
{
	return mag();
}

template<typename T>
T ComplexNumber<T>::mag() const
{
	return std::sqrt(magSqr());
}

template<typename T>
T ComplexNumber<T>::magSqr() const
{
	return mReal*mReal + mImag*mImag;
}

template<typename T>
bool ComplexNumber<T>::hasNaN() const
{
	return std::isnan(mReal) || std::isnan(mImag);
}

template<typename T>
bool ComplexNumber<T>::hasInf() const
{
	return std::isinf(mReal) || std::isinf(mImag);
}

template<typename T>
bool ComplexNumber<T>::isComplex() const
{
	return mImag != 0;
}

template<typename T>
ComplexNumber<T> operator +(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	return ComplexNumber<T>(v1.real() + v2.real(), v1.imag() + v2.imag());
}

template<typename T>
ComplexNumber<T> operator +(const ComplexNumber<T>& v1, T f)
{
	return ComplexNumber<T>(v1.real() + f, v1.imag());
}

template<typename T>
ComplexNumber<T> operator +(T f, const ComplexNumber<T>& v1)
{
	return v1 + f;
}

template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	return ComplexNumber<T>(v1.real() - v2.real(), v1.imag() - v2.imag());
}

template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v1, T f)
{
	return ComplexNumber<T>(v1.real() - f, v1.imag());
}

template<typename T>
ComplexNumber<T> operator -(T f, const ComplexNumber<T>& v1)
{
	return ComplexNumber<T>(f - v1.real(), -v1.imag());
}

template<typename T>
ComplexNumber<T> operator -(const ComplexNumber<T>& v)
{
	return ComplexNumber<T>(-v.real(), -v.imag());
}

template<typename T>
ComplexNumber<T> operator *(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	ComplexNumber<T> tmp = v1;
	return (tmp *= v2);
}

template<typename T>
ComplexNumber<T> operator *(const ComplexNumber<T>& v1, T f)
{
	ComplexNumber<T> tmp = v1;
	return (tmp *= f);
}

template<typename T>
ComplexNumber<T> operator *(T f, const ComplexNumber<T>& v1)
{
	return v1 * f;
}

template<typename T>
ComplexNumber<T> operator /(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	ComplexNumber<T> tmp = v1;
	return (tmp /= v2);
}

template<typename T>
ComplexNumber<T> operator /(const ComplexNumber<T>& v1, T f)
{
	ComplexNumber<T> tmp = v1;
	return (tmp /= f);
}

template<typename T>
ComplexNumber<T> operator /(T f, const ComplexNumber<T>& v1)
{
	ComplexNumber<T> tmp = v1.reciprocal();
	return (tmp *= f);
}

// Comparison
template<typename T>
bool operator ==(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	return v1.real() == v2.real() && v1.imag() == v2.imag();
}

template<typename T>
bool operator !=(const ComplexNumber<T>& v1, const ComplexNumber<T>& v2)
{
	return !(v1 == v2);
}

template<typename T>
bool operator ==(const ComplexNumber<T>& v1, const T& v2)
{
	return !v1.isComplex() && v1.real() == v2;
}

template<typename T>
bool operator !=(const ComplexNumber<T>& v1, T& v2)
{
	return !(v1 == v2);
}

template<typename T>
bool operator ==(const T& v1, const ComplexNumber<T>& v2)
{
	return v2 == v1;
}

template<typename T>
bool operator !=(const T& v1, const ComplexNumber<T>& v2)
{
	return !(v2 == v1);
}

NS_END_NAMESPACE


// Standard functions
namespace std
{
	template<typename T>
	T abs(const NS::ComplexNumber<T>& v)
	{
		return v.abs();
	}

	template<typename T>
	NS::ComplexNumber<T> exp(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = exp(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> log(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = log(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> log10(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = log10(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> pow(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = pow(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> sqrt(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = sqrt(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> sin(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = sin(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> cos(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = cos(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> tan(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = tan(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> asin(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = asin(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> acos(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = acos(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> atan(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = atan(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> sinh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = sinh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> cosh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = cosh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> tanh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = tanh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> asinh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = asinh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> acosh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = acosh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}

	template<typename T>
	NS::ComplexNumber<T> atanh(const NS::ComplexNumber<T>& v)
	{
		complex<T> c = atanh(complex<T>(v.real(), v.imag()));
		return NS::ComplexNumber<T>(c.real(), c.imag());
	}
}