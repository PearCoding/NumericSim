#define NS_ALLOW_CHECKS

#include "Test.h"
#include "quadrature/Quadrature.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(GaussLegendreQuadrature1D)
NS_TEST("1. Order")
{
	GaussLegendreQuadrature<T,1,1> quad;
	T val = quad.eval([](const FixedVector<T,1>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)1);

	val = quad.eval([](const FixedVector<T,1>& x) { return x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);
}
NS_TEST("2. Order")
{
	GaussLegendreQuadrature<T,1,2> quad;
	T val = quad.eval([](const FixedVector<T,1>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)1);

	val = quad.eval([](const FixedVector<T,1>& x) { return x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);

	val = quad.eval([](const FixedVector<T,1>& x) { return x[0]*x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.3333333333333333333);
}
NS_TEST("3. Order")
{
	GaussLegendreQuadrature<T,1,3> quad;
	T val = quad.eval([](const FixedVector<T,1>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)1);

	val = quad.eval([](const FixedVector<T,1>& x) { return x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);

	val = quad.eval([](const FixedVector<T,1>& x) -> T { return x[0]*x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.3333333333333333333);

	val = quad.eval([](const FixedVector<T,1>& x) -> T { return x[0]*x[0]*x[0]; });
	NS_CHECK_NEARLY_EQ(val, (T)0.25);
}
NS_END_TESTCASE()

template<typename T>
NS_BEGIN_TESTCASE_T1(GaussLegendreQuadrature2D)
NS_TEST("1. Order")
{
	GaussLegendreQuadrature<T,2,1> quad;
	T val = quad.eval([](const FixedVector<T,2>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T)x.sum(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.3333333333333333333);
}
NS_TEST("2. Order")
{
	GaussLegendreQuadrature<T,2,2> quad;
	T val = quad.eval([](const FixedVector<T,2>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T)x.sum(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.3333333333333333333);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T)x.magSqr(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.1666666666666666666);
}
NS_TEST("3. Order")
{
	GaussLegendreQuadrature<T,2,3> quad;
	T val = quad.eval([](const FixedVector<T,2>& x) { return (T)1; });
	NS_CHECK_NEARLY_EQ(val, (T)0.5);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T)x.sum(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.3333333333333333333);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T)x.magSqr(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.1666666666666666666);

	val = quad.eval([](const FixedVector<T,2>& x) { return (T) (x*x*x).sum(); });
	NS_CHECK_NEARLY_EQ(val, (T)0.1);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(GaussLegendreQuadrature1D, float);
NST_TESTCASE_T1(GaussLegendreQuadrature1D, double);
NST_TESTCASE_T1(GaussLegendreQuadrature1D, std::complex<double>);

NST_TESTCASE_T1(GaussLegendreQuadrature2D, float);
NST_TESTCASE_T1(GaussLegendreQuadrature2D, double);
NST_TESTCASE_T1(GaussLegendreQuadrature2D, std::complex<double>);
NST_END_MAIN