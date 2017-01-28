#pragma once

#include "Simplex.h"
#include <unordered_set>
#include <unordered_map>

NS_BEGIN_NAMESPACE

/* Mesh representation of a simplical complex */

template<typename T, Dimension K>
class MeshVertex;

template<typename T, Dimension K>
class MeshEdge;

template<typename T, Dimension K>
class MeshElement
{
public:
	Simplex<T,K> Element;
	MeshVertex<T,K>* Vertices[K+1];
	MeshEdge<T,K>* Neighbors[K+1];// Neighboring faces are set by the vertex not in the face

	std::vector<MeshVertex<T,K>*> DOFVertices;

	MeshElement();
};

enum MeshVertexFlags
{
	MVF_StrongBoundary = 0x1,
	MVF_Implicit = 0x2// Node only for shape function, but not with a physical meaning
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
class MeshEdge
{
public:
	MeshVertex<T,K>* Vertices[K];
	MeshElement<T,K>* Elements[2];

	std::vector<MeshVertex<T,K>*> DOFVertices;

	MeshEdge();
};

template<typename T, Dimension K>
class Mesh
{
public:
	typedef std::vector<MeshVertex<T,K>*> MeshVertexList;
	typedef std::vector<MeshElement<T,K>*> MeshElementList;
	typedef std::vector<MeshEdge<T,K>*> MeshEdgeList;

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

	// (Automaticly added after setupNeighbors)
	MeshEdge<T,K>* edge(Index i) const;
	const MeshEdgeList& edges() const;

	// Fourth: (Optional) Automaticly setup boundaries
	void setupBoundaries();

	void prepare();
	void validate() const throw(MeshException);
private:
	struct PrivateData
	{
		MeshElementList Elements;
		MeshVertexList Vertices;
		MeshEdgeList Edges;
		size_t Refs;
	}* mData;
};

NS_END_NAMESPACE


#define _NS_MESH_INL
# include "Mesh.inl"
#undef _NS_MESH_INL