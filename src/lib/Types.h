#pragma once

#include <type_traits>

#include "matrix/Matrix.h"
#include "matrix/SparseMatrix.h"
#include "ComplexNumber.h"

NS_BEGIN_NAMESPACE

template<template<typename, Dimension, Dimension> typename M, typename T, Dimension D1, Dimension D2>
struct is_matrix : std::integral_constant<bool,
	is_number<T>::value &&
		(std::is_same<Matrix<T, D1, D2>, typename std::remove_cv<typename M<T, D1, D2> >::type>::value ||
		std::is_same<SparseMatrix<T, D1, D2>, typename std::remove_cv<typename M<T, D1, D2> >::type>::value)> {};

NS_END_NAMESPACE