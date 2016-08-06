#include "Test.h"
#include "ComplexNumber.h"

template<typename T>
std::ostream& operator<<(std::ostream& out, const NS::ComplexNumber<T>& f)
{
	out << "[ " << f.real() << " " << f.imag() <<  "i ]";
	return out;
}

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Complex)
NS_TEST("default")
{
	ComplexNumber<T> t;
	NS_CHECK_EQ(t.real(), 0);
	NS_CHECK_EQ(t.imag(), 0);
}
NS_TEST("real")
{
	ComplexNumber<T> t = 6;
	NS_CHECK_EQ(t.real(), 6);
	NS_CHECK_EQ(t.imag(), 0);
}
NS_TEST("initializer_list")
{
	ComplexNumber<T> t = { 1, 2 };
	NS_CHECK_EQ(t.real(), 1);
	NS_CHECK_EQ(t.imag(), 2);
}
NS_TEST("+")
{
	ComplexNumber<T> v1(5, 6);
	ComplexNumber<T> v2(1, 2);
	ComplexNumber<T> res(6, 8);
	auto r = v1 + v2;

	NS_CHECK_EQ(r, res);
}
NS_TEST("-")
{
	ComplexNumber<T> v1(5, 6);
	ComplexNumber<T> v2(1, 2);
	ComplexNumber<T> res(4, 4);
	auto r = v1 - v2;

	NS_CHECK_EQ(r, res);
}
NS_TEST("*")
{
	ComplexNumber<T> v1(5, 6);
	ComplexNumber<T> v2(1, 2);
	ComplexNumber<T> res(-7, 16);
	ComplexNumber<T> res2(10, 12);

	auto r = v1 * v2;
	NS_CHECK_EQ(r, res);

	r = v1 * (T)2;
	NS_CHECK_EQ(r, res2);

	r = (T)2 * v1;
	NS_CHECK_EQ(r, res2);
}
NS_TEST("/")
{
	ComplexNumber<T> v1(5, 6);
	ComplexNumber<T> v2(1, 2);
	ComplexNumber<T> res(17/(T)5, -4/(T)5);
	ComplexNumber<T> res2(2.5, 3);
	ComplexNumber<T> res3(2 / (T)5, -4 / (T)5);

	auto r = v1 / v2;
	NS_CHECK_EQ(r, res);

	r = v1 / (T)2;
	NS_CHECK_EQ(r, res2);

	r = (T)2 / v2;
	NS_CHECK_EQ(r, res3);
}
NS_TEST("==")
{
	ComplexNumber<T> v1(5, 6);
	ComplexNumber<T> v2(5, 6);
	ComplexNumber<T> v3(1, 2);

	NS_CHECK_TRUE(v1 == v2);
	NS_CHECK_FALSE(v1 == v3);
	NS_CHECK_TRUE(v1 != v3);
	NS_CHECK_FALSE(v1 != v2);
}
NS_TEST("conjugate")
{
	ComplexNumber<T> v1(1, 2);
	ComplexNumber<T> res(1, -2);
	NS_CHECK_EQ(v1.conjugate(), res);
}
NS_TEST("reciprocal")
{
	ComplexNumber<T> v1(1, 2);
	ComplexNumber<T> res(1/(T)5, -2/(T)5);
	NS_CHECK_EQ(v1.reciprocal(), res);
}
NS_TEST("mag")
{
	ComplexNumber<T> v1(0, 2);
	NS_CHECK_NEARLY_EQ(v1.mag(), 2);
}
NS_TEST("magSqr")
{
	ComplexNumber<T> v1(0, 2);
	NS_CHECK_NEARLY_EQ(v1.magSqr(), 4);
}
NS_TEST("NaN")
{
	ComplexNumber<T> v1(1, NAN);
	NS_CHECK_TRUE(v1.hasNaN());
}
NS_TEST("Inf")
{
	ComplexNumber<T> v1(INFINITY, 2);
	NS_CHECK_TRUE(v1.hasInf());
}
NS_TEST("Complex")
{
	ComplexNumber<T> v1(0, 2);
	NS_CHECK_TRUE(v1.isComplex());

	ComplexNumber<T> v2(0, 0);
	NS_CHECK_FALSE(v2.isComplex());
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Complex, float);
NST_TESTCASE_T1(Complex, double);
NST_END_MAIN