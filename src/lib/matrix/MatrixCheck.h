#pragma once

#include "Types.h"
#include "Utils.h"
#include "MatrixConstructor.h"

NS_BEGIN_NAMESPACE

namespace Check
{
	/**
	 * @brief Checks if the matrix is orthogonal.
	 * @details Orthogonality of a matrix \f$ A \f$ is defined as:
	 * \f[
	 * A^{-1} = A^T \textrm{ or } AA^T = E
	 * \f]
	 * @param m Matrix to check
	 * @return True if orthogonal, false otherwise.
	 */
	template<template<typename> class M, typename T>
	typename std::enable_if<is_dynamic_matrix<M, T>::value, bool >::type
	matrixIsOrthogonal(const M<T>& m);

	template<typename T, Dimension K1, Dimension K2>
	bool matrixIsOrthogonal(const FixedMatrix<T,K1,K2>& m);

	/**
	* @brief Checks if the matrix is unitary.
	* @details Unitary is the complex version of orthogonality and with a matrix \f$ A \f$ it is defined as:
	* \f[
	* A^{-1} = A^* \textrm{ or } AA^* = E
	* \f]
	* @param m Matrix to check
	* @return True if unitary, false otherwise.
	*/
	template<template<typename> class M, typename T>
	typename std::enable_if<is_dynamic_matrix<M, T>::value, bool >::type
	matrixIsUnitary(const M<T>& m);

	template<typename T, Dimension K1, Dimension K2>
	bool matrixIsUnitary(const FixedMatrix<T,K1,K2>& m);

	/**
	* @brief Checks if the matrix is a projection.
	* @details The projection status of a matrix \f$ A \f$ is defined as:
	* \f[
	* A^{2} = A \textrm{ or } AA = A
	* \f]
	* @param m Matrix to check
	* @return True if a projection, false otherwise.
	*/
	template<class M>
	bool matrixIsProjection(const M& m);

	/**
	* @brief Checks if the matrix is symmetric.
	* @details A matrix \f$ A \f$ is symmetric when:
	* \f[
	* A^T = A
	* \f]
	* @param m Matrix to check
	* @return True if symmetric, false otherwise.
	*/
	template<class M>
	bool matrixIsSymmetric(const M& m);

	/**
	* @brief Checks if the matrix is hermitian.
	* @details A matrix \f$ A \f$ is hermitian or complex symmetric when:
	* \f[
	* A^* = A
	* \f]
	* @param m Matrix to check
	* @return True if hermitian, false otherwise.
	*/
	template<class M>
	bool matrixIsHermitian(const M& m);

	/**
	* @brief Checks if the matrix is skew-symmetric.
	* @details A matrix \f$ A \f$ is skew-symmetric when:
	* \f[
	* -A^T = A
	* \f]
	* @param m Matrix to check
	* @return True if skew-symmetric, false otherwise.
	*/
	template<class M>
	bool matrixIsSkewSymmetric(const M& m);

	/**
	* @brief Checks if the matrix is skew-hermitian.
	* @details A matrix \f$ A \f$ is skew-hermitian or complex skew-symmetric when:
	* \f[
	* -A^* = A
	* \f]
	* @param m Matrix to check
	* @return True if skew-hermitian, false otherwise.
	*/
	template<class M>
	bool matrixIsSkewHermitian(const M& m);
}

NS_END_NAMESPACE

#define _NS_MATRIXCHECK_INL
# include "MatrixCheck.inl"
#undef _NS_MATRIXCHECK_INL