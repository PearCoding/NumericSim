#pragma once

#define NS_STRINGIFY(str)	#str
#define NS_DOUBLEQUOTE(str) NS_STRINGIFY(str)

//OS
#if defined(__linux) || defined(linux)
# define NS_OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
# define NS_OS_WINDOWS
# if !defined(Win64) && !defined(_WIN64)
#  define NS_OS_WINDOWS_32
# else
#  define NS_OS_WINDOWS_64
# endif
#else
# error Your operating system is currently not supported
#endif

//Compiler
#if defined(__CYGWIN__)
# define NS_CC_CYGWIN
#endif

#if defined(__GNUC__)
# define NS_CC_GNU
#endif

#if defined(__MINGW32__)
# define NS_CC_MINGW32
#endif

#if defined(__INTEL_COMPILER)
# define NS_CC_INTEL
#endif

#if defined(_MSC_VER)
# define NS_CC_MSC
# pragma warning(disable: 4251 4996)
#endif

#ifndef NS_DEBUG
# ifndef NS_RELEASE
#  define NS_RELEASE
# endif
#endif

#ifdef NS_CC_MSC
# define NS_DEBUG_BREAK() __debugbreak()
# define NS_ALIGN(x) __declspec(align(x))
#else//FIXME: Really use cpu dependent assembler?
# define NS_DEBUG_BREAK() __asm__ __volatile__ ("int $0x03")
# define NS_ALIGN(x) __attribute__((aligned(x)))
#endif

#define NS_NAMESPACE NS
#define NS_BEGIN_NAMESPACE namespace NS_NAMESPACE {
#define NS_END_NAMESPACE }
#define NS_USE_NAMESPACE using namespace NS_NAMESPACE

#ifndef NS_NO_ASSERTS
# define NS_ASSERT(cond) \
	do \
	{ \
		if(!(cond)) \
		{ \
			NS_DEBUG_BREAK(); \
		} \
	} while(0)
#else
# define NS_ASSERT(cond)
#endif

#if defined(NS_DEBUG)
# define NS_DEBUG_ASSERT(cond) NS_ASSERT(cond)
#else
# define NS_DEBUG_ASSERT(cond) NS_NAMESPACE::ns_Noop()
#endif

#define NS_CLASS_NON_MOVEABLE(C) \
	private: \
		C(C&&) = delete; \
		C& operator=(C&&) = delete

#define NS_CLASS_NON_COPYABLE(C) \
	private: \
		C(const C&) = delete; \
		C& operator=(const C&) = delete; \
		NS_CLASS_NON_MOVEABLE(C)

#if !defined(NS_LIB_STATIC)
# if defined(NS_OS_WINDOWS)
#  define NS_EXPORT __declspec(dllexport)
#  define NS_IMPORT __declspec(dllimport)
# elif defined(NS_OS_LINUX)
#  define NS_EXPORT __attribute__((visibility ("default")))
#  define NS_IMPORT
# else
#  error Unsupported operating system
# endif
#else
# define NS_EXPORT
# define NS_IMPORT
#endif

#if defined(NS_LIB_BUILD)
# define NS_LIB PR_EXPORT
#else
# define NS_LIB PR_IMPORT
#endif
#define NS_LIB_INLINE

#include <cmath>
#include <cstring>

NS_BEGIN_NAMESPACE

	inline void ns_Noop()
	{
	}

	typedef signed char int8;
	typedef unsigned char uint8;

	typedef signed short int16;
	typedef unsigned short uint16;

	typedef signed int int32;
	typedef unsigned int uint32;

#if defined __GLIBC_HAVE_LONG_LONG
	__extension__ typedef signed long long int int64;
	__extension__ typedef unsigned long long int uint64;
#else
	typedef signed long int int64;
	typedef unsigned long int uint64;
#endif

	typedef size_t Dimension;
	typedef Dimension Index;

NS_END_NAMESPACE