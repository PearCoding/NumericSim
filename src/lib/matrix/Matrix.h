#pragma once

#include "matrix/SparseMatrix.h"
#include "matrix/FixedMatrix.h"
#include "matrix/DenseMatrix.h"

NS_BEGIN_NAMESPACE

/**
 * @defgroup TypeTraits Type Traits
 * std:: conform type traits to be used by templates.
 */

/**
 * @brief A std:: conform type-trait for the several dense matrix classes.
 * @sa BaseMatrix
 * @ingroup TypeTraits
 */
template<template<typename,Dimension...> class M, typename T, Dimension... Other>
struct is_dense_matrix : std::integral_constant<bool,
	std::is_same<DenseMatrix<T>, typename std::remove_cv<class M<T> >::type>::value ||
		std::is_same<FixedMatrix<T,Other...>, typename std::remove_cv<class M<T,Other...> >::type>::value> {};
	
/**
 * @brief A std:: conform type-trait for the several dynamic matrix classes.
 * @sa DenseMatrix
 * @sa SparseMatrix
 * @ingroup TypeTraits
 */
template<template<typename> class M, typename T>
struct is_dynamic_matrix : std::integral_constant<bool,
	std::is_same<DenseMatrix<T>, typename std::remove_cv<class M<T> >::type>::value ||
		std::is_same<SparseMatrix<T>, typename std::remove_cv<class M<T> >::type>::value> {};

/**
 * @brief A std:: conform type-trait for all the matrix classes.
 * @see is_dense_matrix
 * @sa BaseMatrix
 * @sa SparseMatrix
 * @ingroup TypeTraits
 */
template<template<typename,Dimension...> class M, typename T, Dimension... Other>
struct is_matrix : std::integral_constant<bool,
	is_dense_matrix<M, T, Other...>::value ||
		std::is_same<SparseMatrix<T>, typename std::remove_cv<class M<T> >::type>::value> {};

NS_END_NAMESPACE