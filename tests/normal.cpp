#include "Test.h"
#include "Normal.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Normal)
NS_TEST("2D")
{
	std::array<FixedVector<T,2>,1> arr = {FixedVector<T,2>{1,0}}; 
	FixedVector<T,2> r = {0,-1}; 
	
	NS_CHECK_EQ(Math::normal(arr), r);
}
NS_TEST("3D")
{
	std::array<FixedVector<T,3>,2> arr = {FixedVector<T,3>{1,0,0},FixedVector<T,3>{0,1,0}}; 
	FixedVector<T,3> r = {0,0,1}; 
	
	NS_CHECK_EQ(Math::normal(arr), r);
}
NS_TEST("4D")
{
	std::array<FixedVector<T,4>,3> arr = {
					FixedVector<T,4>{1,0,0,0},
					FixedVector<T,4>{0,1,0,0},
					FixedVector<T,4>{0,0,1,0}}; 
	FixedVector<T,4> r = {0,0,0,-1}; 
	
	NS_CHECK_EQ(Math::normal(arr), r);
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Normal, float);
NST_END_MAIN