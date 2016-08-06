#include "Test.h"
#include "LU.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(LU)
NS_TEST("cholesky")
{
	Matrix<T, 3, 3> m = { {4,12,-16},{12,37,-43},{-16,-43,98} };
	Matrix<T, 3, 3> r = { {2, 0, 0},{6,1,0},{-8,5,3} };

	try
	{
		auto l = LU::serial::cholesky(m);

		NS_CHECK_EQ(l, r);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(LU, float);
NST_TESTCASE_T1(LU, double);
NST_TESTCASE_T1(LU, ComplexNumber<double>);
NST_END_MAIN