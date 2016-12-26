#include "Test.h"
#include "matrix/DenseMatrix.h"
#include "matrix/FixedMatrix.h"
#include "matrix/SparseMatrix.h"
#include "OutputStream.h"

#include "matrix/MatrixConstructor.h"
#include "matrix/MatrixCheck.h"
#include "matrix/MatrixOperations.h"

NS_USE_NAMESPACE;

template<template<typename> class M, typename T>
NS_BEGIN_TESTCASE_T2(Matrix)
NS_TEST("default")
{
	M<T> t(4, 4);
	for (Index i = 0; i < 16; ++i)
		NS_CHECK_EQ(t.linear_at(i), (T)0);
	NS_CHECK_TRUE(t.isEmpty());
}
NS_TEST("set/at")
{
	M<T> t(4, 4);
	t.set(2, 2, (T)1);
	t.set(1, 2, (T)2);
	NS_CHECK_EQ(t.at(2, 2), (T)1);
	NS_CHECK_EQ(t.at(1, 2), (T)2);
	//NS_CHECK_EQ(t.filledCount(), 2);

	t.set(2, 2, (T)0);
	t.set(2, 3, (T)0);
	//NS_CHECK_EQ(t.filledCount(), 1);
	NS_CHECK_EQ(t.at(2, 3), (T)0);

	t.set(3, 3, (T)3);
	NS_CHECK_EQ(t.at(3, 3), (T)3);
	//NS_CHECK_EQ(t.filledCount(), 2);

	t.set(3, 3, (T)4);
	NS_CHECK_EQ(t.at(3, 3), (T)4);
	//NS_CHECK_EQ(t.filledCount(), 2);

	NS_CHECK_FALSE(t.isEmpty());
}
NS_TEST("+")
{
	M<T> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	M<T> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	M<T> r = { { 2, 2, 3 },{ 8, 5, 6 },{ 14, 8, 9 } };
	M<T> t = v1 + v2;

	NS_CHECK_EQ(t, r);
}
NS_TEST("-")
{
	M<T> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	M<T> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	M<T> r = { { 0, -2, 3 },{ 0, -5, 6 },{ 0, -8, 9 } };
	M<T> t = v1 - v2;

	NS_CHECK_EQ(t, r);
}
NS_TEST("*")
{
	M<T> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	M<T> v2 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	M<T> r1 = { { 1, 0, 0 },{ 16, 0, 0 },{ 49, 0, 0 } };
	M<T> r2 = { { 2, 0, 6 },{ 8, 0, 12 },{ 14, 0, 18 } };
	M<T> t = v1 * v2;

	NS_CHECK_EQ(t, r1);

	t = v1 * (T)2;
	NS_CHECK_EQ(t, r2);

	t = (T)2 * v1;
	NS_CHECK_EQ(t, r2);
}
NS_TEST("==")
{
	M<T> v1 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	M<T> v2 = { { 1, 0, 3 },{ 4, 0, 6 },{ 7, 0, 9 } };
	M<T> v3 = { { 1, 2, 0 },{ 4, 5, 0 },{ 7, 8, 0 } };
	
	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_TEST("Initializer List")
{
	M<T> t = { { 1, 2, 3 },{ 4, 5, 0 },{ 7, 8, 9 } };
	NS_CHECK_EQ(t.at(1, 1), (T)5);
	NS_CHECK_EQ(t.at(2, 1), (T)8);
	NS_CHECK_EQ(t.at(1, 2), (T)0);
}
NS_TEST("foreach")
{
	M<T> t = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Index i = 1;
	for (T f : t)
	{
		NS_CHECK_EQ(f, static_cast<T>(i));
		i++;
	}
	NS_CHECK_EQ(i, 10);
}
NS_TEST("row iterator")
{
	M<T> t = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Index i = 4;
	for (auto it = t.row_begin(1); it != t.row_end(1); ++it)
	{
		NS_CHECK_EQ(*it, static_cast<T>(i));
		i++;
	}
	NS_CHECK_EQ(i, 7);
}
NS_TEST("column iterator")
{
	M<T> t = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	Index i = 2;
	for (auto it = t.column_begin(1); it != t.column_end(1); ++it)
	{
		NS_CHECK_EQ(*it, static_cast<T>(i));
		i += 3;
	}
	NS_CHECK_EQ(i, 11);
}
NS_TEST("Transpose")
{
	M<T> t = { { 1, 2 },{ 4, 5 },{ 7, 8 } };
	M<T> r = { { 1, 4, 7 },{ 2, 5, 8 } };
	auto ret = t.transpose();
	NS_CHECK_TRUE(r == ret);
}
NS_TEST("Mul")
{
	M<T> v1 = { { 3, 2, 1 },{ 1, 0, 2 } };
	M<T> v2 = { { 1, 2 },{ 0, 1 },{ 4, 0 } };
	M<T> res = { { 7, 8 },{ 9, 2 } };
	auto ret = v1.mul(v2);
	NS_CHECK_EQ(ret, res);
}
NS_TEST("Mul Vector")
{
	M<T> m = { { 1,2 },{ 0,5 },{ 7,0 } };
	DynamicVector<T> r = { 1,2 };
	DynamicVector<T> l = { 1,2,3 };

	DynamicVector<T> res1 = { 5,10,7 };
	DynamicVector<T> res2 = { 22,12 };
	auto ret1 = m.mul(r);
	auto ret2 = m.mul_left(l);

	NS_CHECK_EQ(ret1, res1);
	NS_CHECK_EQ(ret2, res2);
}
NS_TEST("eye")
{
	M<T> v1 = Construct::eye<M, T>(4, 4);

	for (Index i = 0; i < 4; ++i)
		NS_CHECK_EQ(v1.at(i, i), (T)1);

	NS_CHECK_EQ(v1.trace(), (T)4);
}
NS_TEST("diag")
{
	DynamicVector<T> v = { 1,2,3 };
	M<T> m = Construct::diag<M>(v);
	M<T> res1 = { { 1,0,0 },{ 0,2,0 },{ 0,0,3 } };

	NS_CHECK_EQ(m, res1);
	NS_CHECK_EQ(Construct::diag(m), v);
}
NS_TEST("triu/tril")
{
	M<T> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	M<T> r1 = Construct::triu(m);
	M<T> r2 = Construct::tril(m);

	M<T> res1 = { { 1, 2, 3 },{ 0, 5, 6 },{ 0, 0, 9 } };
	M<T> res2 = { { 1, 0, 0 },{ 4, 5, 0 },{ 7, 8, 9 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("triu/tril k=-1")
{
	M<T> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	M<T> r1 = Construct::triu(m, -1);
	M<T> r2 = Construct::tril(m, -1);

	M<T> res1 = { { 1, 2, 3 },{ 4, 5, 6 },{ 0, 8, 9 } };
	M<T> res2 = { { 0, 0, 0 },{ 4, 0, 0 },{ 7, 8, 0 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("triu/tril k=1")
{
	M<T> m = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	M<T> r1 = Construct::triu(m, 1);
	M<T> r2 = Construct::tril(m, 1);

	M<T> res1 = { { 0, 2, 3 },{ 0, 0, 6 },{ 0, 0, 0 } };
	M<T> res2 = { { 1, 2, 0 },{ 4, 5, 6 },{ 7, 8, 9 } };

	NS_CHECK_EQ(r1, res1);
	NS_CHECK_EQ(r2, res2);
}
NS_TEST("hilbert")
{
	M<T> h = Construct::hilbert<M, T>(3);

	for (Index i = 0; i < 3; ++i)
		for (Index j = 0; j < 3; ++j)
			NS_CHECK_EQ(h.at(i, j), (T)(1 / (i + j + 1.0)));

	M<T> ih = Construct::inv_hilbert<M, T>(3);
	M<T> res = { { 9,-36,30 },{ -36,192,-180 },{ 30,-180,180 } };
	NS_CHECK_EQ(ih, res);

	M<T> e = Construct::eye<M, T>(3,3);
	M<T> m = h.mul(ih);
	NS_CHECK_EQ(m, e);
}
NS_TEST("orthogonal")
{
	M<T> m1 = { {0,1},{1,0} };
	M<T> m2 = { { 1,1 },{ 1,0 } };

	NS_CHECK_TRUE(Check::matrixIsOrthogonal(m1));
	NS_CHECK_FALSE(Check::matrixIsOrthogonal(m2));
}
NS_TEST("unitary")
{
	M<T> m1 = { { 0,1 },{ 1,0 } };// Should be complex 
	M<T> m2 = { { 1,1 },{ 1,0 } };

	NS_CHECK_TRUE(Check::matrixIsUnitary(m1));
	NS_CHECK_FALSE(Check::matrixIsUnitary(m2));
}
NS_TEST("projection")
{
	M<T> m1 = { { 1,0 },{ 0,0 } };
	M<T> m2 = { { 1,1 },{ 1,0 } };

	NS_CHECK_TRUE(Check::matrixIsProjection(m1));
	NS_CHECK_FALSE(Check::matrixIsProjection(m2));
}
NS_TEST("symmetric")
{
	M<T> m1 = { { 0,0,1 },{ 0,1,0 },{ 1,0,0 } };
	M<T> m2 = { { 1,1 },{ 0,1 } };

	NS_CHECK_TRUE(Check::matrixIsSymmetric(m1));
	NS_CHECK_FALSE(Check::matrixIsSymmetric(m2));
}
NS_TEST("hermitian")
{
	M<T> m1 = { { 0,0,1 },{ 0,1,0 },{ 1,0,0 } };// Should be complex
	M<T> m2 = { { 1,1 },{ 0,1 } };

	NS_CHECK_TRUE(Check::matrixIsHermitian(m1));
	NS_CHECK_FALSE(Check::matrixIsHermitian(m2));
}
NS_TEST("skew-symmetric")
{
	M<T> m1 = { { 0,0,1 },{ 0,0,0 },{ -1,0,0 } };
	M<T> m2 = { { 1,1 },{ 0,1 } };

	M<T> m3 = { { 0,0,1 },{ 0,0,0 },{ 1,0,0 } };
	NS_CHECK_EQ(m3.transpose(), m3);

	NS_CHECK_TRUE(Check::matrixIsSkewSymmetric(m1));
	NS_CHECK_FALSE(Check::matrixIsSkewSymmetric(m2));
}
NS_TEST("skew-hermitian")
{
	M<T> m1 = { { 0,0,1 },{ 0,0,0 },{ -1,0,0 } };
	M<T> m2 = { { 1,1 },{ 0,1 } };

	NS_CHECK_TRUE(Check::matrixIsSkewHermitian(m1));
	NS_CHECK_FALSE(Check::matrixIsSkewHermitian(m2));
}
NS_TEST("determinant")
{
	M<T> m = { {1,1,1},{2,3,5},{4,6,8} };
	NS_CHECK_EQ(Operations::determinant(m), (T)-2);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T2(Matrix, DenseMatrix, float);
NST_TESTCASE_T2(Matrix, DenseMatrix, double);
NST_TESTCASE_T2(Matrix, DenseMatrix, ComplexNumber<double>);
NST_TESTCASE_T2(Matrix, SparseMatrix, float);
NST_TESTCASE_T2(Matrix, SparseMatrix, double);
NST_TESTCASE_T2(Matrix, SparseMatrix, ComplexNumber<double>);
NST_END_MAIN