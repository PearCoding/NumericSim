#include "Test.h"

#include "SparseMatrix.h"
#include "ComplexNumber.h"

template<typename T>
std::ostream& operator<<(std::ostream& out, const NS::ComplexNumber<T>& f)
{
	out << "[ " << f.real() << " " << f.imag() << "i ]";
	return out;
}

template<typename T, NS::Dimension D1, NS::Dimension D2>
std::ostream& operator<<(std::ostream& out, const NS::SparseMatrix<T, D1, D2>& f)
{
	out << "[ ";
	for (Index i = 0; i < D2; ++i)
	{
		for (Index j = 0; j < D1; ++j)
			out << f.at(j, i) << " ";

		if (i != D2 - 1)
			out << "| ";
	}
	out << "]";
	return out;
}

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
	SparseMatrix<float, 2, 3> v1 = { { 3, 2, 1 },{ 1, 0, 2 } };
	SparseMatrix<float, 3, 2> v2 = { { 1, 2 },{ 0, 1 },{ 4, 0 } };
	SparseMatrix<float, 2, 2> res = { { 7, 8 },{ 9, 2 } };
	auto ret = v1.mul(v2);
	NS_CHECK_EQ(ret, res);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(SparseMatrix, float);
NST_TESTCASE_T1(SparseMatrix, double);
NST_TESTCASE_T1(SparseMatrix, NS::ComplexNumber<double>);
NST_END_MAIN