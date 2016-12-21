#define NS_ALLOW_CHECKS

#include "Test.h"
#include "Simplex.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Triangle3D)
NS_TEST("volume")
{
	Triangle3D<T> m = { { 7,1,1 }, { 1,0,5 }, { 0,0,1 }};
	NS_CHECK_NEARLY_EQ(m.volume(), (T)14.150971698084906);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Triangle3D, float);
NST_TESTCASE_T1(Triangle3D, double);
NST_TESTCASE_T1(Triangle3D, ComplexNumber<double>);
NST_END_MAIN