#include "Test.h"
#include "Vector.h"
#include "ComplexNumber.h"

template<typename T>
std::ostream& operator<<(std::ostream& out, const NS::ComplexNumber<T>& f)
{
	out << "[ " << f.real() << " " << f.imag() << "i ]";
	return out;
}

template<typename T, NS::Dimension D>
std::ostream& operator<<(std::ostream& out, const NS::Vector<T, D>& f)
{
	out << "[ ";
	for (Index i = 0; i < D; ++i)
		out << f.at(i) << " ";
	out << "]";
	return out;
}

NS_USE_NAMESPACE;

template<typename T, Dimension D>
NS_BEGIN_TESTCASE_T2(Vector)
NS_TEST("default")
{
	Vector<T, D> t;
	for(Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)0);
}
NS_TEST("fill")
{
	Vector<T, D> t(8);
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)8);
}
NS_TEST("+")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(8);
	Vector<T, D> t = v1 + v2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)16);

	t = v1 + (T)2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)10);

	t = (T)2 + v1;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)10);
}
NS_TEST("-")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(8);
	Vector<T, D> t = v1 - v2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)0);

	t = v1 - (T)2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)6);

	t = (T)2 - v1;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)-6);
}
NS_TEST("*")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(8);
	Vector<T, D> t = v1 * v2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)64);

	t = v1 * (T)2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)16);

	t = (T)2 * v1;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)16);
}
NS_TEST("/")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(2);
	Vector<T, D> t = v1 / v2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)4);

	t = v1 / (T)2;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)4);

	t = ((T)4) / v1;
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(t.at(i), (T)0.5);
}
NS_TEST("==")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(8);
	Vector<T, D> v3(6);

	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_TEST("dot")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(2);
	T t = v1.dot(v2);
	NS_CHECK_EQ(t, (T)(16*D));
}
NS_TEST("sum")
{
	Vector<T, D> v1(8);
	T t = v1.sum();
	NS_CHECK_EQ(t, (T)(8 * D));
}
NS_TEST("mag")
{
	Vector<T, D> v1(8);
	T t = v1.mag();
	NS_CHECK_NEARLY_EQ(t, std::sqrt(64 * D));
}
NS_TEST("magSqr")
{
	Vector<T, D> v1(8);
	T t = v1.magSqr();
	NS_CHECK_NEARLY_EQ(t, (T)(64 * D));
}
NS_TEST("swap")
{
	Vector<T, D> v1(8);
	Vector<T, D> v2(4);
	v1.swap(v2);

	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(v1.at(i), (T)4);
	for (Index i = 0; i < D; ++i)
		NS_CHECK_EQ(v2.at(i), (T)8);
}
NS_END_TESTCASE()

NS_BEGIN_TESTCASE(VectorFixed)
NS_TEST("Initializer List")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.at(2), 3);
}
NS_TEST("Sum")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.sum(), 21);
}
NS_TEST("Max")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.max(), 6);
}
NS_TEST("Min")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.min(), 1);
}
NS_TEST("Avg")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	NS_CHECK_EQ(t.avg(), 3.5);
}
NS_TEST("Left")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 4> res = { 1, 2, 3, 4};
	auto l = t.left<3>();
	NS_CHECK_EQ(l, res);
}
NS_TEST("Left Dynamic")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 3> res = { 2, 3, 4 };
	auto l = t.left<3>(3);
	NS_CHECK_EQ(l, res);
}
NS_TEST("Right")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 3> res = { 4, 5, 6 };
	auto l = t.right<3>();
	NS_CHECK_EQ(l, res);
}
NS_TEST("Right Dynamic")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 2> res = { 4, 5 };
	auto l = t.right<2>(3);
	NS_CHECK_EQ(l, res);
}
NS_TEST("Mid")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 3> res = { 3, 4, 5 };
	auto l = t.mid<2, 5>();
	NS_CHECK_EQ(l, res);
}
NS_TEST("Mid Dynamic")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Vector<float, 3> res = { 3, 4, 5 };
	auto l = t.mid<3>(2, 5);
	NS_CHECK_EQ(l, res);
}
NS_TEST("foreach")
{
	Vector<float, 6> t = { 1, 2, 3, 4, 5, 6 };
	Index i = 0;
	for (float f : t)
	{
		NS_CHECK_EQ(f, i + 1);
		i++;
	}
}
NS_TEST("NaN")
{
	Vector<float, 6> t = { 1, NAN, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasNaN());
}
NS_TEST("Inf")
{
	Vector<float, 6> t = { 1, INFINITY, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasInf());
}
NS_TEST("Zero")
{
	Vector<float, 6> t = { 1, 0, 3, 4, 5, 6 };
	NS_CHECK_TRUE(t.hasZero());
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T2(Vector, float, 1);
NST_TESTCASE_T2(Vector, double, 1);
NST_TESTCASE_T2(Vector, float, 8);
NST_TESTCASE_T2(Vector, double, 8);
NST_TESTCASE_T2(Vector, NS::ComplexNumber<double>, 8);
NST_TESTCASE(VectorFixed);
NST_END_MAIN