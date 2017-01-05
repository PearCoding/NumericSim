#pragma once

#include "Simplex.h"
#include <unordered_set>

NS_BEGIN_NAMESPACE

/* Mesh representation of a simplical complex */

template<typename T, Dimension K>
class MeshVertex;

template<typename T, Dimension K>
class MeshElement
{
public:
	struct Neighbor
	{
		MeshElement* Pointer;
		MeshVertex<T,K>* SharedVertices[K];
	};

	Simplex<T,K> Element;
	MeshVertex<T,K>* Vertices[K+1];
	Neighbor Neighbors[K+1];

	MeshElement();
};

enum MeshVertexFlags
{
	MVF_StrongBoundary = 0x1
};

template<typename T, Dimension K>
class MeshVertex
{
public:
	uint32 Flags;
	Index GlobalIndex;
	FixedVector<T,K> Vertex;
	std::vector<MeshElement<T,K>*> Elements;

	MeshVertex();
	MeshVertex(const FixedVector<T,K>& vertex);
};

template<typename T, Dimension K>
class Mesh
{
public:
	typedef std::vector<MeshVertex<T,K>*> MeshVertexList;
	typedef std::vector<MeshElement<T,K>*> MeshElementList;

	Mesh();
	Mesh(const Mesh<T,K>& other);
	Mesh(Mesh<T,K>&& other);
	~Mesh();

	Mesh<T,K>& operator =(const Mesh<T,K>& other);
	Mesh<T,K>& operator =(Mesh<T,K>&& other);

	void clear();

	// First: Add vertices
	void reserveVertices(size_t count);
	void addVertex(MeshVertex<T,K>* vertex);// If successful the owner of the pointer is now the Mesh. Do not delete it
	MeshVertex<T,K>* vertex(Index i) const;
	void setVertex(Index i, MeshVertex<T,K>* vertex);
	const MeshVertexList& vertices() const;

	// Second: Group vertices together and form simplex 
	void reserveElements(size_t count);
	void addElement(MeshElement<T,K>* element);// If successful the owner of the pointer is now the Mesh. Do not delete it
	MeshElement<T,K>* element(Index i) const;
	void setElement(Index i, MeshElement<T,K>* element);
	const MeshElementList& elements() const;

	// Third: After build, setup the neighbors
	void setupNeighbors();

	void prepare();

	void validate() const throw(MeshException);
private:
	struct PrivateData
	{
		MeshElementList Elements;
		MeshVertexList Vertices;
		size_t Refs;
	}* mData;
};

NS_END_NAMESPACE


#define _NS_MESH_INL
# include "Mesh.inl"
#undef _NS_MESH_INL