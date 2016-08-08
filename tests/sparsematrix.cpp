#include "Test.h"

#include "matrix/SparseMatrix.h"
#include "OutputStream.h"

#include "matrix/MatrixConstructor.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(SparseMatrix)
NS_TEST("default")
{
	SparseMatrix<T, 4, 4> t;
	for(Index i = 0; i < 16; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)0);
	NS_CHECK_TRUE(t.isEmpty());
}
NS_TEST("set/at")
{
	SparseMatrix<T, 4, 4> t;
	t.set(2, 2, (T)1);
	t.set(1, 2, (T)2);
	NS_CHECK_EQ(t.at(2, 2), (T)1);
	NS_CHECK_EQ(t.at(1, 2), (T)2);
	NS_CHECK_EQ(t.filledCount(), 2);

	t.set(2, 2, (T)0);
	t.set(2, 3, (T)0);
	NS_CHECK_EQ(t.filledCount(), 1);
	NS_CHECK_EQ(t.at(2, 3), (T)0);

	t.set(3, 3, (T)3);
	NS_CHECK_EQ(t.at(3, 3), (T)3);
	NS_CHECK_EQ(t.filledCount(), 2);

	t.set(3, 3, (T)4);
	NS_CHECK_EQ(t.at(3, 3), (T)4);
	NS_CHECK_EQ(t.filledCount(), 2);

	NS_CHECK_FALSE(t.isEmpty());
}
NS_TEST("+")
{
	SparseMatrix<T, 3, 3> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	SparseMatrix<T, 3, 3> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	SparseMatrix<T, 3, 3> r = { { 2, 2, 3 },{ 8, 5, 6 },{ 14, 8, 9 } };
	SparseMatrix<T, 3, 3> t = v1 + v2;
	
	NS_CHECK_EQ(t, r);
}
NS_TEST("-")
{
	SparseMatrix<T, 3, 3> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	SparseMatrix<T, 3, 3> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	SparseMatrix<T, 3, 3> r = { { 0, -2, 3 },{ 0, -5, 6 },{ 0, -8, 9 } };
	SparseMatrix<T, 3, 3> t = v1 - v2;

	NS_CHECK_EQ(t, r);
}
NS_TEST("*")
{

	SparseMatrix<T, 3, 3> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	SparseMatrix<T, 3, 3> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	SparseMatrix<T, 3, 3> r1 = { { 1, 0, 0 },{ 16, 0, 0 },{ 49, 0, 0 } };
	SparseMatrix<T, 3, 3> r2 = { { 2, 0, 6 },{ 8, 0, 12 },{ 14, 0, 18 } };
	SparseMatrix<T, 3, 3> t = v1 * v2;

	NS_CHECK_EQ(t, r1);

	t = v1 * (T)2;
	NS_CHECK_EQ(t, r2);

	t = (T)2 * v1;
	NS_CHECK_EQ(t, r2);
}
NS_TEST("==")
{
	SparseMatrix<T, 3, 3> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	SparseMatrix<T, 3, 3> v2 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	SparseMatrix<T, 3, 3> v3 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };

	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_TEST("Initializer List")
{
	SparseMatrix<T, 3, 3> t = { {1, 2, 3}, {4, 5, 0}, {7, 8, 9} };
	NS_CHECK_EQ(t.at(1, 1), (T)5);
	NS_CHECK_EQ(t.at(2, 1), (T)8);
	NS_CHECK_EQ(t.at(1, 2), (T)0);
}
NS_TEST("foreach")
{
	SparseMatrix<T, 3, 3> t = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Index i = 0;
	for (T f : t)
	{
		NS_CHECK_EQ(f, (T)(i + 1));
		i++;
	}
}
NS_TEST("Transpose")
{
	SparseMatrix<T, 3, 2> t = { { 1, 2 },{ 4, 5 },{ 7, 8 } };
	SparseMatrix<T, 2, 3> r = { { 1, 4, 7 },{ 2, 5, 8 } };
	auto ret = t.transpose();
	NS_CHECK_TRUE(r == ret);
}
NS_TEST("Mul")
{
	SparseMatrix<T, 2, 3> v1 = { { 3, 2, 1 },{ 1, 0, 2 } };
	SparseMatrix<T, 3, 2> v2 = { { 1, 2 },{ 0, 1 },{ 4, 0 } };
	SparseMatrix<T, 2, 2> res = { { 7, 8 },{ 9, 2 } };
	auto ret = v1.mul(v2);
	NS_CHECK_EQ(ret, res);
}
NS_TEST("Mul Vector")
{
	SparseMatrix<float, 3, 2> m = { { 1,2 },{ 0,5 },{ 7,0 } };
	Vector<float, 2> r = { 1,2 };
	Vector<float, 3> l = { 1,2,3 };

	Vector<float, 3> res1 = { 5,10,7 };
	Vector<float, 2> res2 = { 22,12 };
	auto ret1 = m.mul(r);
	auto ret2 = m.mul_left(l);

	NS_CHECK_EQ(ret1, res1);
	NS_CHECK_EQ(ret2, res2);
}
NS_TEST("eye")
{
	SparseMatrix<T, 4, 4> v1 = Construct::eye<SparseMatrix, T, 4, 4>();

	for (Index i = 0; i < 4; ++i)
		NS_CHECK_EQ(v1.at(i, i), (T)1);

	NS_CHECK_EQ(v1.trace(), (T)4);
}
NS_TEST("diag")
{
	Vector<T, 3> v = { 1,2,3 };
	SparseMatrix<T, 3, 3> m = Construct::diag<SparseMatrix>(v);
	SparseMatrix<T, 3, 3> res1 = { { 1,0,0 },{ 0,2,0 },{ 0,0,3 } };

	NS_CHECK_EQ(m, res1);
	NS_CHECK_EQ(Construct::diag(m), v);
}
NS_TEST("hilbert")
{
	SparseMatrix<T, 3, 3> h = Construct::hilbert<SparseMatrix, T, 3, 3>();

	for (Index i = 0; i < 3; ++i)
		for (Index j = 0; j < 3; ++j)
			NS_CHECK_EQ(h.at(i, j), (T)(1 / (i + j + 1.0)));

	SparseMatrix<T, 3, 3> ih = Construct::inv_hilbert<SparseMatrix, T, 3, 3>();
	SparseMatrix<T, 3, 3> res = { { 9,-36,30 },{ -36,192,-180 },{ 30,-180,180 } };
	NS_CHECK_EQ(ih, res);

	SparseMatrix<T, 3, 3> e = Construct::eye<SparseMatrix, T, 3, 3>();
	SparseMatrix<T, 3, 3> m = h.mul(ih);
	NS_CHECK_EQ(m, e);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(SparseMatrix, float);
NST_TESTCASE_T1(SparseMatrix, double);
NST_TESTCASE_T1(SparseMatrix, NS::ComplexNumber<double>);
NST_END_MAIN