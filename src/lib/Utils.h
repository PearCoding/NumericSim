#pragma once

#include "Config.h"

NS_BEGIN_NAMESPACE

/*
 Unfortunately some constexpr functions are available only since c++14,
 but we have the baseline c++11.
*/
template<typename T>
constexpr T t_min(T v1, T v2)
{
	return !(v2 < v1) ? v1 : v2;
}

template<typename T>
constexpr T t_max(T v1, T v2)
{
	return (v1 < v2) ? v2 : v1;
}

namespace Math
{

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, int8>::type
		sign_pow(const T& k)
	{
		return (k % 2 == 0) ? 1 : -1;
	}

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type
		binom(const T& n, typename std::make_unsigned<T>::type k)
	{
		typedef typename std::make_unsigned<T>::type k_t;
		if (k == 0)
			return 1;

		if (2 * (T)k > n)//Really cast to T? What is with signed?
			return binom(n, static_cast<k_t>(n - k));

		T v = n - k + 1;
		for (k_t i = 2; i <= k; ++i)
		{
			v *= (n - k + i);
			v /= i;
		}

		return v;
	}

	template<typename T>
	typename std::enable_if<!std::is_integral<T>::value, T>::type 
		binom(const T& n, uint32 k)
	{
		T v = (T)1;
		for (uint32 i = 0; i < k; ++i)
		{
			double inv = 1.0 / (i + 1);
			v *= (n - (T)i) * (T)inv;
		}

		return v;
	}
}

NS_END_NAMESPACE