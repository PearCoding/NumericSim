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

	m = { { 12,6 }, { -1,10 }, { 0,6 }};
	m.prepare();
	NS_CHECK_NEARLY_EQ(m.determinant(), (T)48);
	NS_CHECK_NEARLY_EQ(m.volume(), (T)24);
}
NS_TEST("global")
{
	Triangle<T> m = { { 7,1 }, { 1,0 }, { 0,0 }};
	FixedVector<T,2> r = {2,0.25};
	m.prepare();
	NS_CHECK_NEARLY_EQ_V(m.toGlobal(FixedVector<T,2>{0.25,0.5}), r);
}
NS_TEST("local")
{
	Triangle<T> m = { { 7,1 }, { 1,0 }, { 0,0 }};
	FixedVector<T,2> r = {0.25,0.5};
	m.prepare();
	NS_CHECK_NEARLY_EQ_V(m.toLocal(FixedVector<T,2>{2,0.25}), r);
}
NS_TEST("radius")
{
	Triangle<T> m = { { 0,0 }, { 1,0 }, { 0,1 }};// Standard simplex
	FixedVector<T,2> c = {1/(double)3,1/(double)3};
	m.prepare();
	NS_CHECK_NEARLY_EQ_V(m.center(), c);
	NS_CHECK_NEARLY_EQ((m[0]-m.center()).mag(), (T)1);
	NS_CHECK_NEARLY_EQ((m[1]-m.center()).mag(), (T)1);
	NS_CHECK_NEARLY_EQ((m[2]-m.center()).mag(), (T)1);
	NS_CHECK_NEARLY_EQ(m.outerRadius(), (T)1);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Triangle, float);
NST_TESTCASE_T1(Triangle, double);
NST_TESTCASE_T1(Triangle, std::complex<double>);
NST_END_MAIN