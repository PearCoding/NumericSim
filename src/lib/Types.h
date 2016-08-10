#pragma once

#include <type_traits>

#include "matrix/Matrix.h"
#include "matrix/SparseMatrix.h"
#include "ComplexNumber.h"

NS_BEGIN_NAMESPACE

/**
 * @defgroup TypeTraits Type Traits
 * std:: conform type traits to be used by templates.
 */

/**
 * @brief A std:: conform type-trait for the several matrix classes.
 * @sa Matrix
 * @sa SparseMatrix
 * @ingroup TypeTraits
 */
template<template<typename> class M, typename T>
struct is_matrix : std::integral_constant<bool,
	is_number<T>::value &&
		(std::is_same<Matrix<T>, typename std::remove_cv<class M<T> >::type>::value ||
		std::is_same<SparseMatrix<T>, typename std::remove_cv<class M<T> >::type>::value)> {};

NS_END_NAMESPACE