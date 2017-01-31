#define NS_ALLOW_CHECKS

#include "Test.h"
#include "LU.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<template<typename> class M, typename T>
NS_BEGIN_TESTCASE_T2(LU)
NS_TEST("cholesky")
{
	M<T> m = { {4,12,-16},{12,37,-43},{-16,-43,98} };
	M<T> r = { {2, 0, 0},{6,1,0},{-8,5,3} };

	try
	{
		M<T> l(3,3);
		LU::serial::cholesky(m,l);

		NS_CHECK_EQ(l, r);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_TEST("doolittle")
{
	M<T> A = { {1,1,1},{2,3,5},{4,6,8} };
	M<T> L = { {1, 0, 0},{0.25,1,0},{0.5,0,1} };
	M<T> U = { {4, 6, 8},{0,-0.5,-1},{0,0,1} };
	M<T> P = { {0, 0, 1},{1,0,0},{0,1,0} };

	try
	{
		M<T> rL(3,3), rU(3,3), rP(3,3);
		size_t pivotCount;
		LU::serial::doolittle(A, rL, rU, rP, &pivotCount);

		NS_CHECK_EQ(rL, L);
		NS_CHECK_EQ(rU, U);
		NS_CHECK_EQ(rP, P);
		NS_CHECK_EQ(pivotCount, 2);
		NS_CHECK_EQ(rL.mul(rU), P.mul(A));
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_TEST("solve_lu")
{
	M<T> A = { {1, 2, 0},{0,1,0},{0,4,2} };
	DynamicVector<T> b = { 1,2,4 };
	DynamicVector<T> r = {-3,2,-2};

	try
	{
		M<T> rL(3,3), rU(3,3), rP(3,3);
		size_t pivotCount;
		LU::serial::doolittle(A, rL, rU, rP, &pivotCount);
		
		DynamicVector<T> x = LU::serial::solve_lu(rL,rU,rP.mul(b));
		NS_CHECK_NEARLY_EQ_V(x, r);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

template<typename T>
NS_BEGIN_TESTCASE_T1(LU_SparseOnly)
NS_TEST("ilu0")
{
	SparseMatrix<T> A = { 
		{1,-1,-1,-1},
		{-1,2,0,0},
		{-1,0,3,1},
		{-1,0,1,4}
	};

	SparseMatrix<T> L = { 
		{1, 0, 0, 0},
		{-1,1,0,0},
		{-1,0,1,0},
		{-1,0,0,1}
	};

	SparseMatrix<T> U = { 
		{1, -1, -1, -1},
		{0,1,0,0},
		{0,0,2,0},
		{0,0,0,3}
	};
	
	try
	{
		SparseMatrix<T> rL(4,4), rU(4,4);
		size_t pivotCount;
		LU::serial::ilu0(A, rL, rU);

		NS_CHECK_EQ(rL, L);
		NS_CHECK_EQ(rU, U);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T2(LU, DenseMatrix, float);
NST_TESTCASE_T2(LU, DenseMatrix, double);
NST_TESTCASE_T2(LU, DenseMatrix, std::complex<double>);
NST_TESTCASE_T2(LU, SparseMatrix, float);
NST_TESTCASE_T2(LU, SparseMatrix, double);
NST_TESTCASE_T2(LU, SparseMatrix, std::complex<double>);

NST_TESTCASE_T1(LU_SparseOnly, float);
NST_END_MAIN