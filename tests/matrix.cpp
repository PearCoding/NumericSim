#include "Test.h"
#include "Matrix.h"

template<typename T, NS::Dimension D1, NS::Dimension D2>
std::ostream& operator<<(std::ostream& out, const NS::Matrix<T, D1, D2>& f)
{
	out << "[ ";
	for (Index i = 0; i < D1; ++i)
	{
		for (Index j = 0; j < D2; ++j)
			out << f.at(i, j) << " ";

		if (i != D1 - 1)
			out << "| ";
	}
	out << "]";
	return out;
}

NS_USE_NAMESPACE;

template<typename T, Dimension D1, Dimension D2>
NS_BEGIN_TESTCASE_T3(Matrix)
NS_TEST("default")
{
	Matrix<T, D1, D2> t;
	for(Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 0);
}
NS_TEST("fill")
{
	Matrix<T, D1, D2> t(8);
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 8);
}
NS_TEST("+")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 + v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 16);
}
NS_TEST("-")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 - v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 0);
}
NS_TEST("*")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 * v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 64);

	t = v1 * (T)2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 16);

	t = (T)2 * v1;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), 16);
}
NS_TEST("==")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> v3(6);

	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_END_TESTCASE()

NS_BEGIN_TESTCASE(MatrixFixed)
NS_TEST("Initializer List")
{
	Matrix<float, 3, 3> t = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
	NS_CHECK_EQ(t.at(1, 1), 5);
}
NS_TEST("foreach")
{
	Matrix<float, 3, 3> t = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Index i = 0;
	for (float f : t)
	{
		NS_CHECK_EQ(f, i + 1);
		i++;
	}
}
NS_TEST("Transpose")
{
	Matrix<float, 3, 2> t = { { 1, 2 },{ 4, 5 },{ 7, 8 } };
	Matrix<float, 2, 3> r = { { 1, 4, 7 },{ 2, 5, 8 } };
	auto ret = t.transpose();
	NS_CHECK_EQ(r, ret);
}
NS_TEST("Mul")
{
	Matrix<float, 2, 3> v1 = { { 3, 2, 1 },{ 1, 0, 2 } };
	Matrix<float, 3, 2> v2 = { { 1, 2 },{ 0, 1 },{ 4, 0} };
	Matrix<float, 2, 2> res = { { 7, 8 },{ 9, 2 } };
	auto ret = v1.mul(v2);
	NS_CHECK_EQ(ret, res);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T3(Matrix, float, 1, 1);
NST_TESTCASE_T3(Matrix, double, 1, 1);
NST_TESTCASE_T3(Matrix, float, 4, 4);
NST_TESTCASE_T3(Matrix, double, 4, 4);
NST_TESTCASE(MatrixFixed);
NST_END_MAIN