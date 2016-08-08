#include "Test.h"
#include "matrix/Matrix.h"
#include "OutputStream.h"

#include "matrix/MatrixConstructor.h"

NS_USE_NAMESPACE;

template<typename T, Dimension D1, Dimension D2>
NS_BEGIN_TESTCASE_T3(Matrix)
NS_TEST("default")
{
	Matrix<T, D1, D2> t;
	for(Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)0);
}
NS_TEST("fill")
{
	Matrix<T, D1, D2> t(8);
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)8);
}
NS_TEST("+")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 + v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)16);
}
NS_TEST("-")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 - v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)0);
}
NS_TEST("*")
{
	Matrix<T, D1, D2> v1(8);
	Matrix<T, D1, D2> v2(8);
	Matrix<T, D1, D2> t = v1 * v2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)64);

	t = v1 * (T)2;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)16);

	t = (T)2 * v1;
	for (Index i = 0; i < D1*D2; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)16);
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
NS_TEST("eye")
{
	Matrix<T, D1, D2> v1 = Construct::eye<Matrix, T, D1, D2>();

	for (Index i = 0; i < std::min(D1, D2); ++i)
		NS_CHECK_EQ(v1.at(i, i), (T)1);

	NS_CHECK_EQ(v1.trace(), static_cast<T>(std::min(D1, D2)));
}
NS_TEST("hilbert")
{
	Matrix<T, D1, D2> v1 = Construct::hilbert<Matrix, T, D1, D2>();

	for (Index i = 0; i < D1; ++i)
		for(Index j = 0; j < D2; ++j)
			NS_CHECK_EQ(v1.at(i, j), (T)(1/(i + j + 1.0)));
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
NS_TEST("Mul Vector")
{
	Matrix<float, 3, 2> m = { { 1,2 },{ 4,5 },{7,8} };
	Vector<float, 2> r = { 1,2 };
	Vector<float, 3> l = { 1,2,3 };

	Vector<float, 3> res1 = { 5,14,23 };
	Vector<float, 2> res2 = { 30,36 };
	auto ret1 = m.mul(r);
	auto ret2 = m.mul_left(l);

	NS_CHECK_EQ(ret1, res1);
	NS_CHECK_EQ(ret2, res2);
}
NS_TEST("diag")
{
	Vector<float, 3> v = { 1,2,3 };
	Matrix<float, 3, 3> m = Construct::diag<Matrix>(v);
	Matrix<float, 3, 3> res1 = { {1,0,0},{0,2,0},{0,0,3} };

	NS_CHECK_EQ(m, res1);
	NS_CHECK_EQ(Construct::diag(m), v);
}
NS_TEST("triu/tril")
{
	Matrix<float, 3, 3> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Matrix<float, 3, 3> r1 = Construct::triu(m);
	Matrix<float, 3, 3> r2 = Construct::tril(m);

	Matrix<float, 3, 3> res1 = { { 1, 2, 3 },{ 0, 5, 6 },{ 0, 0, 9 } };
	Matrix<float, 3, 3> res2 = { { 1, 0, 0 },{ 4, 5, 0 },{ 7, 8, 9 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("triu/tril k=-1")
{
	Matrix<float, 3, 3> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Matrix<float, 3, 3> r1 = Construct::triu(m,-1);
	Matrix<float, 3, 3> r2 = Construct::tril(m,-1);

	Matrix<float, 3, 3> res1 = { { 1, 2, 3 },{ 4, 5, 6 },{ 0, 8, 9 } };
	Matrix<float, 3, 3> res2 = { { 0, 0, 0 },{ 4, 0, 0 },{ 7, 8, 0 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("triu/tril k=1")
{
	Matrix<float, 3, 3> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Matrix<float, 3, 3> r1 = Construct::triu(m,1);
	Matrix<float, 3, 3> r2 = Construct::tril(m,1);

	Matrix<float, 3, 3> res1 = { { 0, 2, 3 },{ 0, 0, 6 },{ 0, 0, 0 } };
	Matrix<float, 3, 3> res2 = { { 1, 2, 0 },{ 4, 5, 6 },{ 7, 8, 9 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("Inverse Hilbert")
{
	Matrix<float, 3, 3> ret = Construct::inv_hilbert<Matrix, float, 3, 3>();
	Matrix<float, 3, 3> res = { {9,-36,30},{-36,192,-180},{30,-180,180} };
	NS_CHECK_EQ(ret, res);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T3(Matrix, float, 1, 1);
NST_TESTCASE_T3(Matrix, double, 1, 1);
NST_TESTCASE_T3(Matrix, float, 4, 4);
NST_TESTCASE_T3(Matrix, double, 4, 4);
NST_TESTCASE_T3(Matrix, ComplexNumber<double>, 4, 4);
NST_TESTCASE(MatrixFixed);
NST_END_MAIN