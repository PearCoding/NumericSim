#define NS_ALLOW_CHECKS

#include "Test.h"
#include "Iterative.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

constexpr uint32 MAX_ITERATIONS = 1024;
constexpr double ITER_EPSILON = 10e-9;

template<typename T>
NS_BEGIN_TESTCASE_T1(Iterative)
NS_TEST("jacobi")
{
	Matrix<T> m = { {4,1,2},{1,3,2},{1,1,2} };
	Vector<T> b = { 12, 13, 9 };
	Vector<T> x0(3);
	Vector<T> res = { 1, 2, 3 };

	uint32 iterations;
	auto l = Iterative::serial::jacobi(m, b, x0, MAX_ITERATIONS, ITER_EPSILON, &iterations);
	std::cout << "Iterations: " << iterations << std::endl;
	NS_CHECK_EQ(l, res);
}
NS_TEST("gauss-seidel")
{
	Matrix<T> m = { { 4,1,2 },{ 1,3,2 },{ 1,1,2 } };
	Vector<T> b = { 12, 13, 9 };
	Vector<T> x0(3);
	Vector<T> res = { 1, 2, 3 };

	uint32 iterations;
	auto l = Iterative::serial::sor(m, b, x0, (T)1, MAX_ITERATIONS, ITER_EPSILON, &iterations);
	std::cout << "Iterations: " << iterations << std::endl;
	NS_CHECK_EQ(l, res);
}
NS_TEST("sor")
{
	Matrix<T> m = { { 4,1,2 },{ 1,3,2 },{ 1,1,2 } };
	Vector<T> b = { 12, 13, 9 };
	Vector<T> x0(3);
	Vector<T> res = { 1, 2, 3 };

	uint32 iterations;
	auto l = Iterative::serial::sor(m, b, x0, (T)1.1, MAX_ITERATIONS, ITER_EPSILON, &iterations);
	std::cout << "Iterations: " << iterations << std::endl;
	NS_CHECK_EQ(l, res);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Iterative, float);
NST_TESTCASE_T1(Iterative, double);
NST_TESTCASE_T1(Iterative, ComplexNumber<double>);
NST_END_MAIN