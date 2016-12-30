#pragma once

#include "Vector.h"
#include "Simplex.h"

NS_BEGIN_NAMESPACE

/**
 * @brief Calculates the Gauss-Legendre quadrature points-
 */
template<typename T, Dimension K, Dimension Order>
class GaussLegendreQuadratureFactory
{
public:
	GaussLegendreQuadratureFactory();

	const std::vector<FixedVector<T,K> >& getQuadraturePoints() const;
	const std::vector<T>& getQuadratureWeights() const;

private:
	std::vector<FixedVector<T,K> > mPoints;
	std::vector<T> mWeights;
};

NS_END_NAMESPACE


#define _NS_GAUSSLEGENDREQUADRATUREFACTORY_INL
# include "GaussLegendreQuadratureFactory.inl"
#undef _NS_GAUSSLEGENDREQUADRATUREFACTORY_INL