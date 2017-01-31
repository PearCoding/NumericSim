#pragma once

#include <queue>
#include <map>

#include "SparseMatrix.h"
#include "Vector.h"
#include "Utils.h"

NS_BEGIN_NAMESPACE

/**
* @brief Applies the CutHill-McKee algorithm
* @param m Should be a square symmetric sparse matrix
* @return Returns new ordering of matrix
*/
template<typename T>
DynamicVector<Index> cuthill_mckee(const SparseMatrix<T>& m);

/**
* @brief Applies the reverse CutHill-McKee algorithm
* @param m Should be a square symmetric sparse matrix
* @return Returns new ordering of matrix
*/
template<typename T>
DynamicVector<Index> r_cuthill_mckee(const SparseMatrix<T>& m);

template<typename T>
SparseMatrix<T> permutation_matrix(const DynamicVector<Index>& permutation);

DynamicVector<Index> inverse_permutation(const DynamicVector<Index>& permutation);

/**
* @brief Reorders the given matrix based on the permuation
* @param m Should be a square sparse matrix
* @return Returns new matrix
* @todo Need an inplace algorithm and faster algorithm.
*/
template<typename T>
SparseMatrix<T> permutate(const SparseMatrix<T>& m, const DynamicVector<Index>& permutation);

template<typename T>
DynamicVector<T> permutate(const DynamicVector<T>& m, const DynamicVector<Index>& permutation);

NS_END_NAMESPACE

#define _NS_MATRIXORDER_INL
# include "MatrixOrder.inl"
#undef _NS_MATRIXORDER_INL