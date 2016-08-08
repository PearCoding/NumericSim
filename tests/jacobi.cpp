#include "Test.h"
#include "Jacobi.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Jacobi)
NS_TEST("jacobi")
{
	Matrix<T, 3, 3> m = { {2,0,8},{0,4,6},{0,6,2} };
	Vector<T, 3> b = { 1, 2, 3 };
	Vector<T, 3> x0;
	Vector<T, 3> res = { 0.5, 0.5, 0 };

	uint32 iterations;
	auto l = Iterative::serial::jacobi(m, b, x0, 1024, 10e-5, &iterations);
	std::cout << "Iterations: " << iterations << std::endl;
	NS_CHECK_EQ(l, res);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Jacobi, float);
NST_TESTCASE_T1(Jacobi, double);
NST_TESTCASE_T1(Jacobi, ComplexNumber<double>);
NST_END_MAIN