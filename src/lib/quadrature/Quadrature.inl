#ifndef _NS_QUADRATURE_INL
# error Quadrature.inl should only be included by Quadrature.h
#endif


NS_BEGIN_NAMESPACE

template<template<typename,Dimension,Dimension> class Factory, typename T, Dimension K, Dimension Order>
template<class F, typename RT>
RT Quadrature<Factory, T, K, Order>::eval(const F& func, const RT& start) const
{
	const std::vector<FixedVector<T,K> >& points = this->getQuadraturePoints();
	const std::vector<T>& weights = this->getQuadratureWeights();

	NS_ASSERT(points.size() == weights.size());

	RT v = start;
	for(Index i = 0; i < points.size(); ++i)
		v += weights[i] * func(points[i]);

	return v;
}

NS_END_NAMESPACE