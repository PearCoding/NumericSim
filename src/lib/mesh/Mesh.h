#pragma once

#include "Simplex.h"
#include <unordered_set>

NS_BEGIN_NAMESPACE

namespace Mesh
{
	/* Mesh representation of a simplical complex */

	template<typename T, Dimension K, Dimension N>
	class MeshVertex;

	template<typename T, Dimension K, Dimension N>
	class MeshSimplex
	{
	public:
		struct Neighbor
		{
			MeshSimplex* Pointer;
			MeshVertex<T,K,N>* SharedVertices[K];
		};

		MeshVertex<T,K,N>* Vertices[K+1];
		Neighbor Neighbors[K+1];

		MeshSimplex();

		Simplex<T,K,N> toSimplex() const;
	};

	template<typename T, Dimension K, Dimension N>
	class MeshVertex
	{
	public:
		FixedVector<T,N> Vertex;
		std::vector<MeshSimplex<T,K,N>*> Simplices;

		MeshVertex();
		MeshVertex(const FixedVector<T,N>& vertex);
	};

	template<typename T, Dimension K, Dimension N>
	class Mesh
	{
	public:
		typedef std::vector<MeshVertex<T,K,N>*> MeshVertexList;
		typedef std::vector<MeshSimplex<T,K,N>*> MeshSimplexList;

		Mesh();
		~Mesh();

		void clear();

		// First: Add vertices
		void reserveVertices(size_t count);
		void addVertex(MeshVertex<T,K,N>* vertex);// If successful the owner of the pointer is now the Mesh. Do not delete it
		MeshVertex<T,K,N>* vertex(Index i) const;
		void setVertex(Index i, MeshVertex<T,K,N>* vertex);
		const MeshVertexList& vertices() const;

		// Second: Group vertices together and form simplex 
		void reserveSimplices(size_t count);
		void addSimplex(MeshSimplex<T,K,N>* simplex);// If successful the owner of the pointer is now the Mesh. Do not delete it
		MeshSimplex<T,K,N>* simplex(Index i) const;
		void setSimplex(Index i, MeshSimplex<T,K,N>* simplex);
		const MeshSimplexList& simplices() const;

		// Third: After build, setup the neighbors
		void setupNeighbors();

		void validate() const throw(MeshException);
	private:
		MeshSimplexList mSimplices;
		MeshVertexList mVertices;
	};
}

NS_END_NAMESPACE


#define _NS_MESH_INL
# include "Mesh.inl"
#undef _NS_MESH_INL