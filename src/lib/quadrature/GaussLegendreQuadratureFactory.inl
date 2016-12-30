#ifndef _NS_GAUSSLEGENDREQUADRATUREFACTORY_INL
# error GaussLegendreQuadratureFactory.inl should only be included by GaussLegendreQuadratureFactory.h
#endif


NS_BEGIN_NAMESPACE

/**
 * Due to time constraints we will be using precomputed values.
 * See the paper for the 2d triangle values:
 *   Quadrature Formulas in Two Dimensions
 *   Math 5172 - Finite Element Method
 *   Section 001, Spring 2010
 *
 * http://math2.uncc.edu/~shaodeng/TEACHING/math5172/Lectures/Lect_15.PDF
 */
template<typename T, Dimension K, Dimension Order>
GaussLegendreQuadratureFactory<T, K, Order>::GaussLegendreQuadratureFactory()
{
	static_assert(K == 1 || K == 2,
		"Gauss Legendre for dimension other than 1 or 2 currently not implemented.");
	static_assert(Order >= 1 && Order <= 3,
		"Gauss Legendre currently only for orders between 1 and 3 implemented.");

	switch(K)
	{
	case 1:
		switch(Order)
		{
		case 1:
			mPoints.push_back(FixedVector<T,K>{0.5});
			mWeights.push_back(1);
			break;
		case 2:
			mPoints.push_back(FixedVector<T,K>{0.211324865405187});
			mWeights.push_back(0.5);

			mPoints.push_back(FixedVector<T,K>{0.788675134594813});
			mWeights.push_back(0.5);
			break;
		case 3:
			mPoints.push_back(FixedVector<T,K>{0.5});
			mWeights.push_back(0.444444444444444);

			mPoints.push_back(FixedVector<T,K>{0.112701665379258});
			mWeights.push_back(0.277777777777777);

			mPoints.push_back(FixedVector<T,K>{0.887298334620742});
			mWeights.push_back(0.277777777777777);
			break;
		}
		break;
	case 2:
		switch(Order)
		{
		case 1:
			mPoints.push_back(FixedVector<T,K>{0.333333333333333, 0.333333333333333});
			mWeights.push_back(1);
			break;
		case 2:
			mPoints.push_back(FixedVector<T,K>{0, 0.5});
			mWeights.push_back(0.333333333333333);

			mPoints.push_back(FixedVector<T,K>{0.5, 0});
			mWeights.push_back(0.333333333333333);

			mPoints.push_back(FixedVector<T,K>{0.5, 0.5});
			mWeights.push_back(0.333333333333333);
			break;
		case 3:
			mPoints.push_back(FixedVector<T,K>{0.333333333333333, 0.333333333333333});
			mWeights.push_back(-0.562500000000000);

			mPoints.push_back(FixedVector<T,K>{0.2, 0.2});
			mWeights.push_back(0.520833333333333);

			mPoints.push_back(FixedVector<T,K>{0.2, 0.6});
			mWeights.push_back(0.520833333333333);

			mPoints.push_back(FixedVector<T,K>{0.6, 0.2});
			mWeights.push_back(0.520833333333333);
			break;
		}
		break;
	}

	// Normalize to the unit volume of the K-Simplex
	const auto norm = Simplex<T,K>::unitVolume();
	for(Index i = 0; i < mWeights.size(); ++i)
		mWeights[i] *= norm;
}

template<typename T, Dimension K, Dimension Order>
const std::vector<FixedVector<T,K> >& GaussLegendreQuadratureFactory<T, K, Order>::getQuadraturePoints() const
{
	return mPoints;
}

template<typename T, Dimension K, Dimension Order>
const std::vector<T>& GaussLegendreQuadratureFactory<T, K, Order>::getQuadratureWeights() const
{
	return mWeights;
}

NS_END_NAMESPACE