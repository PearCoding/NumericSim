#pragma once

#include "Types.h"
#include "Utils.h"

NS_BEGIN_NAMESPACE

namespace Construct
{
	/**
	* @brief Generates a identity matrix with the given dimensions.
	* @param d1 Row dimension
	* @param d2 Column dimension
	* @return A new matrix with the size `d1 x d2` and ones at the diagonal.
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		eye(Dimension d1, Dimension d2);

	/**
	* @brief Generates a square matrix with the given diagonal element.
	* @param v The vector representing the diagonal of the new matrix.
	* @return A new matrix with the size `v.size() x v.size()` and vector v at the diagonal.
	*/
	template<template<typename> class M, typename T, typename DC>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		diag(const Vector<T,DC>& v);

	/**
	* @brief Returns the diagonal of the matrix as a vector.
	* @param v A matrix.
	* @return A new vector with the size `min(m.rows(),m.columns())` and the diagonal of the matrix as his content.
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, DynamicVector<T> >::type
		diag(const M<T>& m);

	/**
	* @brief Returns the lower left triangle matrix of a matrix.
	* @param m A matrix.
	* @param k The offset from the diagonal (=0). > 0 means more to the upper right, < 0 means more to the lower left.
	* @return A new matrix with the same size as m.
	* @sa triu
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		tril(const M<T>& m, int32 k = 0);

	/**
	* @brief Returns the upper right triangle matrix of a matrix.
	* @param m A matrix.
	* @param k The offset from the diagonal (=0). > 0 means more to the upper right, < 0 means more to the lower left.
	* @return A new matrix with the same size as m.
	* @sa tril
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		triu(const M<T>& m, int32 k = 0);

	/**
	* @brief Returns the hilbert matrix.
	* @details The hilbert matrix is based on the formula:
	* \f$ H_{ij} = \frac{1}{i + j + 1} \f$
	* @param d The dimension of the hilbert matrix.
	* @return A new matrix with the dimension d.
	* @sa inv_hilbert
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		hilbert(Dimension d);

	/**
	* @brief Returns the inverse hilbert matrix.
	* @details The inverse hilbert matrix is based on the formula:
	* \f$ (H^{-1})_{ij} = (-1)^{i+j} (i+j+1) \binom{d+i}{d-j-1} \binom{d+j}{d-i-1} {\binom{i+j}{i}}^2 \f$
	* @param d The dimension of the hilbert matrix.
	* @return A new matrix with the dimension d.
	* @sa hilbert
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_matrix<M, T>::value, M<T> >::type
		inv_hilbert(Dimension d);
}

NS_END_NAMESPACE

#define _NS_MATRIXCONSTRUCTOR_INL
# include "MatrixConstructor.inl"
#undef _NS_MATRIXCONSTRUCTOR_INL