#pragma once

#include "Types.h"
#include "Vector.h"
#include "matrix/MatrixOperations.h"

NS_BEGIN_NAMESPACE

template<typename T, Dimension K, Dimension N>
class Simplex
{	
public:
	FixedVector<T,N> vertices[K+1];
	
	inline Simplex();
	inline Simplex(std::initializer_list<std::initializer_list<T> > list);

	inline T volume() const;
	inline Simplex gradient() const;
};

// Typedefs
template<typename T, Dimension N>
using Triangle = Simplex<T, 2, N>;
template<typename T>
using Triangle2D = Triangle<T, 2>;
template<typename T>
using Triangle3D = Triangle<T, 3>;

NS_END_NAMESPACE


#define _NS_SIMPLEX_INL
# include "Simplex.inl"
#undef _NS_SIMPLEX_INL