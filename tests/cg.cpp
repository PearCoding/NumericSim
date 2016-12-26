#define NS_ALLOW_CHECKS

#include "Test.h"
#include "CG.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

constexpr uint32 MAX_ITERATIONS = 1024;
constexpr double ITER_EPSILON = 10e-9;

template<typename T>
NS_BEGIN_TESTCASE_T1(CG)
NS_TEST("cg")
{
	DenseMatrix<T> m = { { 4,1 },{ 1,3 } };
	DynamicVector<T> b = { 1,2 };
	DynamicVector<T> x0 = { 2,1 };
	DynamicVector<T> res = { 1/11.0, 7/11.0 };

	size_t iterations;
	try
	{
		auto l = CG::serial::cg(m, b, x0, MAX_ITERATIONS, ITER_EPSILON, &iterations);
		std::cout << "Iterations: " << iterations << std::endl;
		NS_CHECK_EQ(l, res);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(CG, float);
NST_TESTCASE_T1(CG, double);
//NST_TESTCASE_T1(CG, ComplexNumber<double>);
NST_END_MAIN