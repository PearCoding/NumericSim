#include "Test.h"
#include "Utils.h"

NS_USE_NAMESPACE;

NS_BEGIN_TESTCASE(Utils)
NS_TEST("sign_pow")
{
	NS_CHECK_EQ(Math::sign_pow(0), 1);
	NS_CHECK_EQ(Math::sign_pow(1), -1);
	NS_CHECK_EQ(Math::sign_pow(2), 1);
	NS_CHECK_EQ(Math::sign_pow(-1), -1);
	NS_CHECK_EQ(Math::sign_pow(-2), 1);
}
NS_TEST("binom")
{
	NS_CHECK_EQ(Math::binom(0, 0), 1);
	NS_CHECK_EQ(Math::binom(2, 1), 2);
	NS_CHECK_EQ(Math::binom(4, 2), 6);
	NS_CHECK_EQ(Math::binom(7, 7), 1);
	NS_CHECK_EQ(Math::binom(2.5, 2), 1.875);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE(Utils);
NST_END_MAIN