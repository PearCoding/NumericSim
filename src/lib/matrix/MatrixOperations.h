#pragma once

#include "Utils.h"
#include "LU.h"

NS_BEGIN_NAMESPACE

namespace Operations
{
	/**
	* @brief Calculates the determinant of the matrix m
	* @param m Square Matrix or square Sparse Matrix
	* @return 0 if non square or singular, determinant else
	*/
	template<template<typename> class M, typename T>
	T determinant(const M<T>& m);
	
	template<typename T, Dimension K1, Dimension K2>
	T determinant(const FixedMatrix<T, K1, K2>& m);
}

NS_END_NAMESPACE

#define _NS_MATRIXOPERATIONS_INL
# include "MatrixOperations.inl"
#undef _NS_MATRIXOPERATIONS_INL