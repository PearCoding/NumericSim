#pragma once

#include "GaussLegendreQuadratureFactory.h"

NS_BEGIN_NAMESPACE

/**
 * @brief Calculates the quadrature on the standard K-Simplex.
 */
template<template<typename,Dimension,Dimension> class Factory, typename T, Dimension K, Dimension Order>
class Quadrature : Factory<T,K,Order>
{
public:
	Quadrature() : Factory<T,K,Order>() {}

	template<class F, typename RT = typename std::result_of<F(FixedVector<T,K>)>::type>
	RT eval(const F& func, const RT& start = (RT)0) const;
};

template<typename T, Dimension K, Dimension Order>
using GaussLegendreQuadrature = Quadrature<GaussLegendreQuadratureFactory, T, K, Order>;

NS_END_NAMESPACE


#define _NS_QUADRATURE_INL
# include "Quadrature.inl"
#undef _NS_QUADRATURE_INL