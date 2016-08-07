#pragma once

#include "Config.h"

NS_BEGIN_NAMESPACE

class NSException
{
public:
	NSException() {}
	virtual const char* what() const = 0;
};

#define NS_DECLARE_EXCEPTION_GROUP(name, super) \
	class name ## Exception : public super ## Exception {}

#define NS_DECLARE_EXCEPTION(name, group, description) \
	class name ## Exception : public group ## Exception { \
	public: \
		const char* what() const final { return description; } \
	}

NS_DECLARE_EXCEPTION_GROUP(Math, NS);
NS_DECLARE_EXCEPTION(DivisionByZero, Math, "Division by Zero is not defined.");
NS_DECLARE_EXCEPTION(NotPositiveDefinite, Math, "Matrix is not positive definite.");

NS_END_NAMESPACE