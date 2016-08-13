#pragma once

#include "Config.h"

NS_BEGIN_NAMESPACE

/**
* @defgroup Exceptions Exceptions
* Exception types used in NumericSim
*/

/**
 * @brief Top level exception class.
 * All other exceptions derive from this class.
 * @exception NSException General NumericSim library exception
 * @ingroup Exceptions
 */
class NSException
{
public:
	NSException() {}

	/**
	 * @brief Returns an English description of the exception.
	 * @return An English zero-terminated string.
	 */
	virtual const char* what() const = 0;
};

#define NS_DECLARE_EXCEPTION_GROUP(name, super) \
	class name ## Exception : public super ## Exception {}

#define NS_DECLARE_EXCEPTION(name, group, description) \
	class name ## Exception : public group ## Exception { \
	public: \
		const char* what() const final { return description; } \
	}

NS_DECLARE_EXCEPTION_GROUP(Init, NS);
NS_DECLARE_EXCEPTION(MatrixInitializerListFailed, Init, "Tried to initialize matrix with an ill-formed initializer-list.");

NS_DECLARE_EXCEPTION_GROUP(Math, NS);
NS_DECLARE_EXCEPTION(DivisionByZero, Math, "Division by Zero is not defined.");
NS_DECLARE_EXCEPTION(MatrixHasZeroInDiag, Math, "Matrix has a zero in his diagonal.");
NS_DECLARE_EXCEPTION(NotSquare, Math, "Matrix is not square.");
NS_DECLARE_EXCEPTION(NotPositiveDefinite, Math, "Matrix is not positive definite.");
NS_DECLARE_EXCEPTION(NotHermitian, Math, "Matrix is not hermitian.");
NS_DECLARE_EXCEPTION(IterativeMatrixVectorMismatch, Math, "Matrix and vectors for iterative methods do not match in dimensions.");

NS_DECLARE_EXCEPTION_GROUP(Dimension, NS);
NS_DECLARE_EXCEPTION(VectorSizeMismatch, Dimension, "Size of vectors do not match.");
NS_DECLARE_EXCEPTION(MatrixSizeMismatch, Dimension, "Size of matrix do not match.");
NS_DECLARE_EXCEPTION(MatrixMulMismatch, Dimension, "Dimensional requirement for multiplication not fulfilled.");

NS_END_NAMESPACE