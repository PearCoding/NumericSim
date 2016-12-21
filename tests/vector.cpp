#include "Test.h"
#include "Vector.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Vector)
NS_TEST("default")
{
	FixedVector<T,8> t;
	for(Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)0);
}
NS_TEST("fill")
{
	FixedVector<T,8> t;
	t.fill(8);
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)8);
}
NS_TEST("+")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(8);
	FixedVector<T,8> t = v1 + v2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)16);

	t = v1 + (T)2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)10);

	t = (T)2 + v1;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)10);
}
NS_TEST("-")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(8);
	FixedVector<T,8> t = v1 - v2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)0);

	t = v1 - (T)2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)6);

	t = (T)2 - v1;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)-6);
}
NS_TEST("*")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(8);
	FixedVector<T,8> t = v1 * v2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)64);

	t = v1 * (T)2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)16);

	t = (T)2 * v1;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)16);
}
NS_TEST("/")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(2);
	FixedVector<T,8> t = v1 / v2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)4);

	t = v1 / (T)2;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)4);

	t = ((T)4) / v1;
	for (Index i = 0; i < t.size(); ++i)
		NS_CHECK_EQ(t.at(i), (T)0.5);
}
NS_TEST("==")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(8);
	FixedVector<T,8> v3;
	v3.fill(6);

	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_TEST("dot")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(2);
	T t = v1.dot(v2);
	NS_CHECK_EQ(t, (T)(16*v1.size()));
}
NS_TEST("sum")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	T t = v1.sum();
	NS_CHECK_EQ(t, (T)(8 * v1.size()));
}
NS_TEST("mag")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	T t = v1.mag();
	NS_CHECK_NEARLY_EQ(t, std::sqrt(64 * v1.size()));
}
NS_TEST("magSqr")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	T t = v1.magSqr();
	NS_CHECK_NEARLY_EQ(t, (T)(64 * v1.size()));
}
NS_TEST("swap")
{
	FixedVector<T,8> v1;
	v1.fill(8);
	FixedVector<T,8> v2;
	v2.fill(4);
	v1.swap(v2);

	for (Index i = 0; i < v1.size(); ++i)
		NS_CHECK_EQ(v1.at(i), (T)4);
	for (Index i = 0; i < v2.size(); ++i)
		NS_CHECK_EQ(v2.at(i), (T)8);
}
NS_END_TESTCASE()
template<typename T>
NS_BEGIN_TESTCASE_T1(DynamicVector)
NS_TEST("Initializer List")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.at(2), (T)3);
}
NS_TEST("Sum")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.sum(), (T)21);
}
NS_TEST("Max")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.max(), (T)6);
}
NS_TEST("Min")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.min(), (T)1);
}
NS_TEST("Avg")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.avg(), (T)3.5);
}
NS_TEST("Left")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	DynamicVector<T> res = { 1, 2, 3, 4};
	auto l = t.left(3);
	NS_CHECK_EQ(l, res);
}
NS_TEST("Right")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	DynamicVector<T> res = { 4, 5, 6 };
	auto l = t.right(3);
	NS_CHECK_EQ(l, res);
}
NS_TEST("Mid")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	DynamicVector<T> res = { 3, 4, 5 };
	auto l = t.mid(2, 5);
	NS_CHECK_EQ(l, res);
}
NS_TEST("foreach")
{
	DynamicVector<T> t = { 1, 2, 3, 4, 5, 6 };
	Index i = 0;
	for (T f : t)
	{
		NS_CHECK_EQ(f, (T)(i + 1));
		i++;
	}
	NS_CHECK_EQ(i, 6);
}
NS_TEST("NaN")
{
	DynamicVector<T> t = { 1, NAN, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasNaN());
}
NS_TEST("Inf")
{
	DynamicVector<T> t = { 1, INFINITY, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasInf());
}
NS_TEST("Zero")
{
	DynamicVector<T> t = { 1, 0, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasZero());
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Vector, float);
NST_TESTCASE_T1(Vector, double);
NST_TESTCASE_T1(Vector, NS::ComplexNumber<double>);
NST_TESTCASE_T1(DynamicVector, float);
NST_TESTCASE_T1(DynamicVector, double);
NST_TESTCASE_T1(DynamicVector, NS::ComplexNumber<double>);
NST_END_MAIN