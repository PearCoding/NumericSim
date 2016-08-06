#pragma once

#include <type_traits>

#include "Matrix.h"
#include "SparseMatrix.h"
#include "ComplexNumber.h"

NS_BEGIN_NAMESPACE

template<typename M, typename T, Dimension D1, Dimension D2>
struct is_matrix : std::integral_constant<bool,
	is_number<T>::value &&
		(std::is_same<Matrix<T, D1, D2>, typename std::remove_cv<M>::type>::value ||
		std::is_same<SparseMatrix<T, D1, D2>, typename std::remove_cv<M>::type>::value)> {};

NS_END_NAMESPACE