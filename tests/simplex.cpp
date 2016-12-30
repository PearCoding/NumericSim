#define NS_ALLOW_CHECKS

#include "Test.h"
#include "Simplex.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Triangle)
NS_TEST("volume")
{
	Triangle<T> m = { { 7,1 }, { 1,0 }, { 0,0 }};
	m.prepare();
	NS_CHECK_NEARLY_EQ(m.determinant(), (T)-1);
	NS_CHECK_NEARLY_EQ(m.volume(), (T)0.5000);
}
NS_TEST("global")
{
	Triangle<T> m = { { 7,1 }, { 1,0 }, { 0,0 }};
	FixedVector<T,2> r = {2,0.25};
	m.prepare();
	NS_CHECK_NEARLY_EQ_V(m.toGlobal(FixedVector<T,2>{0.25,0.5}), r);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Triangle, float);
NST_TESTCASE_T1(Triangle, double);
NST_TESTCASE_T1(Triangle, std::complex<double>);
NST_END_MAIN