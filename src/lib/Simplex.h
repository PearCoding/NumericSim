#pragma once

#include "Types.h"
#include "Vector.h"
#include "matrix/MatrixOperations.h"

NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
class Simplex
{	
public:
	static constexpr Dimension Order = K;
	static constexpr Dimension VertexCount = K+1;

	typedef FixedVector<T,K> vertex_t;
	typedef FixedMatrix<T,K,K> matrix_t;
	
	Simplex();
	Simplex(std::initializer_list<std::initializer_list<T> > list);

	void prepare();

	const vertex_t& operator[](Index i) const;
	vertex_t& operator[](Index i);

	static T unitVolume();
	T volume() const;

	vertex_t center() const;
	// Radius of the circumscribed hypersphere
	T outerRadius() const;

	//vertex_t toLocal(const vertex_t& global) const;
	vertex_t toGlobal(const vertex_t& local) const;

	matrix_t gradient() const;

	vertex_t faceCenter(Index i) const;
	vertex_t faceNormal(Index i) const;// Jump Vector

	const matrix_t& matrix() const;
	//const matrix_t& inverseMatrix() const;
	T determinant() const;

private:
	vertex_t mVertices[K+1];
	bool mPrepared;
	matrix_t mMatrix;
	//matrix_t mInverseMatrix;
	T mDeterminant;
};

// Typedefs
template<typename T>
using Triangle = Simplex<T, 2>;

NS_END_NAMESPACE


#define _NS_SIMPLEX_INL
# include "Simplex.inl"
#undef _NS_SIMPLEX_INL