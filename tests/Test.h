#pragma once

#include "Config.h"

#include <iostream>
#include <list>
#include <limits>
#include <string>
#include <sstream>

namespace NST
{
	class Test
	{
	private:
		struct Entry
		{
			std::string MSG;
			std::string FUNCTION;
			int LINE;
		};

		std::list<Entry> mErrors;
		int mCount;
		std::string mName;

	public:
		inline explicit Test(const std::string& name) :
			mName(name)
		{
			mCount = 0;
		}

		inline void assert(bool cond, const std::string& msg, const std::string& func, int line)
		{
			if (!cond)
			{
				add_error(msg, func, line);
			}
			mCount++;
		}

		inline bool end() const 
		{
			std::cout << "Test '" << mName << "':" << std::endl;
			int i = 1;
			for(Entry e : mErrors)
			{
				std::cout << "  #" << i << " (" << e.FUNCTION << ": " << e.LINE << ")" << std::endl
					<< "    " << e.MSG << std::endl;
				i++;
			}
			std::cout << "----------------------" << std::endl;
			std::cout << "  " << (mCount - mErrors.size()) << "/" << mCount << " checks successful." << std::endl;

			return mErrors.size() == 0;
		}
	private:
		inline void add_error(const std::string& msg, const std::string& func, int line)
		{
			mErrors.push_back({ msg, func, line });
		}
	};

	class TestCase
	{
	public:
		inline explicit TestCase(const std::string& name) :
			mCount(0), mErrors(0)
		{
			std::cout << "Test case: " << name << std::endl;
		}

		inline TestCase(const std::string& name, const std::string& t1) :
			mCount(0), mErrors(0)
		{
			std::cout << "Test case: " << name  << "<" << t1 << ">" << std::endl;
		}

		inline TestCase(const std::string& name, const std::string& t1, const std::string& t2) :
			mCount(0), mErrors(0)
		{
			std::cout << "Test case: " << name << "<" << t1 << "," << t2 << ">" << std::endl;
		}

		inline TestCase(const std::string& name, const std::string& t1, const std::string& t2, const std::string& t3) :
			mCount(0), mErrors(0)
		{
			std::cout << "Test case: " << name << "<" << t1 << "," << t2 << "," << t3 << ">" << std::endl;
		}

		inline Test* begin(const std::string& name)
		{
			return new Test(name);
		}

		inline void end(Test* test)
		{
			mCount++;
			std::cout << "**********************" << std::endl;
			std::cout << mCount << ". ";
			if (!test->end())
				mErrors++;
			std::cout << std::endl;

			delete test;
		}

		inline bool end()
		{
			std::cout << "**********************" << std::endl;
			std::cout << "Result: " << (mCount - mErrors) << "/" << mCount << " successful." << std::endl;

			return mErrors == 0;
		}

	private:
		int mCount;
		int mErrors;
	};
}

#ifndef NST_EPSILON
# define NST_EPSILON (std::numeric_limits<float>::epsilon())
#endif

#define NS_BEGIN_TESTCASE(name) \
	int _testcase_ ##name() { \
	NST::TestCase _testcase(NS_DOUBLEQUOTE(name)); \
	NST::Test* _test = nullptr;

#define NS_BEGIN_TESTCASE_T1(name) \
	int _testcase_ ##name(const std::string& t1) { \
	NST::TestCase _testcase(NS_DOUBLEQUOTE(name), t1); \
	NST::Test* _test = nullptr;

#define NS_BEGIN_TESTCASE_T2(name) \
	int _testcase_ ##name(const std::string& t1, const std::string& t2) { \
	NST::TestCase _testcase(NS_DOUBLEQUOTE(name), t1, t2); \
	NST::Test* _test = nullptr;

#define NS_BEGIN_TESTCASE_T3(name) \
	int _testcase_ ##name(const std::string& t1, const std::string& t2, const std::string& t3) { \
	NST::TestCase _testcase(NS_DOUBLEQUOTE(name), t1, t2, t3); \
	NST::Test* _test = nullptr;

#define NS_END_TESTCASE() \
	if (_test) \
		_testcase.end(_test); \
	return _testcase.end(); \
	}

#define NST_TESTCASE_F(name) \
	_testcase_ ##name()

#define NST_TESTCASE_F_T1(name, t) \
	_testcase_ ##name<t>(NS_STRINGIFY(t))

#define NST_TESTCASE_F_T2(name, t1, t2) \
	_testcase_ ##name<t1, t2>(NS_STRINGIFY(t1), NS_STRINGIFY(t2))

#define NST_TESTCASE_F_T3(name, t1, t2, t3) \
	_testcase_ ##name<t1, t2, t3>(NS_STRINGIFY(t1), NS_STRINGIFY(t2), NS_STRINGIFY(t3))

#define NS_TEST(name) \
	if (_test)\
		_testcase.end(_test);\
	_test = _testcase.begin(name);


#define NS_FUNCTION_NAME __func__

#define NS_GOT_EXCEPTION(e) \
	{ \
		std::stringstream _stream; \
		_stream << "EXCEPTION: " << (e).what(); \
		_test->assert(false, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected " << (expected) << " but got " << (value); \
		_test->assert((value) == (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NOT_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Not expected " << (expected) << " but got it"; \
		_test->assert((value) != (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_LESS(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected less then " << (expected) << " but got " << (value); \
		_test->assert((value) < (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_LESS_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected less or equal then " << (expected) << " but got " << (value); \
		_test->assert((value) <= (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_GREAT(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected greater then " << (expected) << " but got " << (value); \
		_test->assert((value) < (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_GREAT_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected greater or equal then " << (expected) << " but got " << (value); \
		_test->assert((value) <= (expected), _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NEARLY_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected (nearly) " << (expected) << " but got " << (value); \
		_test->assert(std::abs((value) - (expected)) <= NST_EPSILON, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NEARLY_EQ_V(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected (nearly) " << (expected) << " but got " << (value); \
		_test->assert(std::abs(((value) - (expected)).sum()) <= NST_EPSILON, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NOT_NEARLY_EQ(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Not expected (nearly) " << (expected) << " but got it"; \
		_test->assert(std::abs((value) - (expected)) > NST_EPSILON, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NOT_NEARLY_EQ_V(value, expected) \
	{ \
		std::stringstream _stream; \
		_stream << "Not expected (nearly) " << (expected) << " but got it"; \
		_test->assert(std::abs(((value) - (expected)).sum()) > NST_EPSILON, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NULL(value) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected NULL but not got it"; \
		_test->assert((value) == nullptr, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_NOT_NULL(value) \
	{ \
		std::stringstream _stream; \
		_stream << "Not expected NULL but got it"; \
		_test->assert((value) != nullptr, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_TRUE(value) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected true but got false"; \
		_test->assert((value) == true, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NS_CHECK_FALSE(value) \
	{ \
		std::stringstream _stream; \
		_stream << "Expected false but got true"; \
		_test->assert((value) == false, _stream.str(), NS_FUNCTION_NAME, __LINE__); \
	}

#define NST_BEGIN_MAIN \
	int main(int argc, char** argv) \
	{ \
		int _errors = 0;

#define NST_TESTCASE(name) \
		_errors += NST_TESTCASE_F(name) ? 0 : 1

#define NST_TESTCASE_T1(name, t1) \
		_errors += NST_TESTCASE_F_T1(name, t1) ? 0 : 1

#define NST_TESTCASE_T2(name, t1, t2) \
		_errors += NST_TESTCASE_F_T2(name, t1, t2) ? 0 : 1

#define NST_TESTCASE_T3(name, t1, t2, t3) \
		_errors += NST_TESTCASE_F_T3(name, t1, t2, t3) ? 0 : 1

#define NST_END_MAIN \
		NS_DEBUG_ASSERT(_errors == 0); \
		std::cout << "######################" << std::endl; \
		if(_errors == 0) \
			std::cout << "Status: Successful" << std::endl; \
		else \
			std::cout << "Status: " << _errors << " errors" << std::endl; \
		return -_errors; \
	}
