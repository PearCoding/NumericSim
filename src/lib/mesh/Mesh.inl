#ifndef _NS_MESH_INL
# error Mesh.inl should only be included by Mesh.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
MeshElement<T,K>::MeshElement() :
	Element()
{
	for(Index i = 0; i < K+1; ++i)
	{
		Vertices[i] = nullptr;
		Neighbors[i] = nullptr;
	}
}

//------------------------------------------------
template<typename T, Dimension K>
MeshVertex<T,K>::MeshVertex() :
	Flags(0), GlobalIndex(0), Vertex()
{}

template<typename T, Dimension K>
MeshVertex<T,K>::MeshVertex(const FixedVector<T,K>& vertex) :
	Flags(0), GlobalIndex(0), Vertex(vertex)
{}

//------------------------------------------------
template<typename T, Dimension K>
MeshEdge<T,K>::MeshEdge()
{
	for(Index i = 0; i < K-1; ++i)
		Vertices[i] = nullptr;
	
	Elements[0] = nullptr;
	Elements[1] = nullptr;
}

//------------------------------------------------
template<typename T, Dimension K>
Mesh<T,K>::Mesh()
{
	mData = new PrivateData;
	mData->Refs = 1;
}

template<typename T, Dimension K>
Mesh<T,K>::Mesh(const Mesh<T,K>& other)
{
	mData = other.mData;
	mData->Refs++;
}

template<typename T, Dimension K>
Mesh<T,K>::Mesh(Mesh<T,K>&& other)
{
	mData = other.mData;
	other.mData = nullptr;
}

template<typename T, Dimension K>
Mesh<T,K>::~Mesh()
{
	if(mData)
	{
		NS_ASSERT(mData->Refs > 0);
		mData->Refs--;
		if(mData->Refs == 0)
		{
			clear();
			delete mData;
		}
	}
}

template<typename T, Dimension K>
Mesh<T,K>& Mesh<T,K>::operator =(const Mesh<T,K>& other)
{
	if(mData)
	{
		NS_ASSERT(mData->Refs > 0);
		mData->Refs--;
		if(mData->Refs == 0)
		{
			clear();
			delete mData;
		}
	}

	mData = other.mData;
	mData->Refs++;
	
	return *this;
}

template<typename T, Dimension K>
Mesh<T,K>& Mesh<T,K>::operator =(Mesh<T,K>&& other)
{
	if(mData)
	{
		NS_ASSERT(mData->Refs > 0);
		mData->Refs--;
		if(mData->Refs == 0)
		{
			clear();
			delete mData;
		}
	}

	mData = other.mData;
	other.mData = nullptr;

	return *this;
}
	
template<typename T, Dimension K>
void Mesh<T,K>::clear()
{
	for(auto ptr : mData->Vertices)
		delete ptr;
	mData->Vertices.clear();
	
	for(auto ptr : mData->Elements)
		delete ptr;
	mData->Elements.clear();
}

template<typename T, Dimension K>
void Mesh<T,K>::reserveVertices(size_t count)
{
	NS_ASSERT(count > 0);
	mData->Vertices.reserve(count);
}

template<typename T, Dimension K>
void Mesh<T,K>::addVertex(MeshVertex<T,K>* vertex)
{
	NS_ASSERT(vertex);
	vertex->GlobalIndex = mData->Vertices.size();
	mData->Vertices.push_back(vertex);
}

template<typename T, Dimension K>
MeshVertex<T,K>* Mesh<T,K>::vertex(Index i) const
{
	return mData->Vertices[i];
}

template<typename T, Dimension K>
void Mesh<T,K>::setVertex(Index i, MeshVertex<T,K>* vertex)
{
	mData->Vertices[i] = vertex;
}

template<typename T, Dimension K>
const typename Mesh<T,K>::MeshVertexList& Mesh<T,K>::vertices() const
{
	return mData->Vertices;
}

template<typename T, Dimension K>
void Mesh<T,K>::reserveElements(size_t count)
{
	NS_ASSERT(count > 0);
	mData->Elements.reserve(count);
}

template<typename T, Dimension K>
void Mesh<T,K>::addElement(MeshElement<T,K>* element)
{
	NS_ASSERT(element);

	mData->Elements.push_back(element);

	for(Index i = 0; i < K+1; ++i)
	{
		element->Element[i] = element->Vertices[i]->Vertex;
		element->Vertices[i]->Elements.push_back(element);
	}
}

template<typename T, Dimension K>
MeshElement<T,K>* Mesh<T,K>::element(Index i) const
{
	return mData->Elements[i];
}

template<typename T, Dimension K>
void Mesh<T,K>::setElement(Index i, MeshElement<T,K>* element)
{
	mData->Elements[i] = element;
}

template<typename T, Dimension K>
const typename Mesh<T,K>::MeshElementList& Mesh<T,K>::elements() const
{
	return mData->Elements;
}

template<typename T, Dimension K>
MeshEdge<T,K>* Mesh<T,K>::edge(Index i) const
{
	return mData->Edges[i];
}

template<typename T, Dimension K>
const typename Mesh<T,K>::MeshEdgeList& Mesh<T,K>::edges() const
{
	return mData->Edges;	
}

template<typename T, Dimension K>
void Mesh<T,K>::setupNeighbors()
{
	for(const auto& S : mData->Elements)
	{
		size_t nextEntry = 0;

		for(Index i = 0; i < K+1; ++i)
		{
			if(S->Neighbors[i])// Already set
				continue;

			size_t sharedVertices = 0;
			MeshVertex<T,K>* shared[K];
			for(Index j = 0; j < K+1; ++j)
			{
				if(i == j)
					continue;

				shared[sharedVertices] = S->Vertices[j];
				sharedVertices++;
			}

			if(sharedVertices != K)
				throw MalformedElementNeighborConnectionException();

			// Create edge
			MeshEdge<T,K>* edge = new MeshEdge<T,K>();
			edge->Elements[0] = S;

			for(Index j = 0; j < K; ++j)
				edge->Vertices[j] = shared[j];
			
			S->Neighbors[i] = edge;

			mData->Edges.push_back(edge);

			// Find corresponding other element if exists
			for(Index j = 0; j < K; ++j)
			{
				// Iterate through all vertices of other element
				for(auto oe : shared[j]->Elements)
				{
					if(oe == S)
						continue;
						
					size_t matches = 0;
					for(auto ov : oe->Vertices)
					{
						for(Index k = 0; k < K; ++k)
						{
							if(shared[k] == ov)
							{
								matches++;
								break;
							}
						}
					}

					if(matches == K)
					{
						if(edge->Elements[1] && edge->Elements[1] != oe)
							throw TooManySharedFacesException();
						
						edge->Elements[1] = oe;
					}
				}
			}

			// For symmetric reasons set found element neighbor too
			if(edge->Elements[1])
			{
				// First find the corresponding vertex not in the edge
				for(Index j = 0; j < K+1; ++j)
				{
					bool found = false;
					for(Index k = 0; k < K; ++k)
					{
						if(shared[k] == edge->Elements[1]->Vertices[j])
						{
							found = true;
							break;
						}
					}

					// Add if found
					if(!found)
					{
						edge->Elements[1]->Neighbors[j] = edge;
						break;
					}
				}
			}
		}
	}
}

template<typename T, Dimension K>
void Mesh<T,K>::prepare()
{
	for(auto s : mData->Elements)
		s->Element.prepare();
}

template<typename T, Dimension K>
void Mesh<T,K>::validate() const throw(MeshException)
{
	Index id = 0;
	for(auto v : mData->Vertices)
	{
		if(v->GlobalIndex != id)
			throw InvalidVertexIndexException();
		++id;

		// Every vertex has to be connected to atleast one simplex
		if(v->Elements.empty())
			throw IsolatedVertexException();

		// Every connected simplex has to contain the vertex itself (dev)
		for(auto s : v->Elements)
		{
			bool found = false;
			for(Index i = 0; i < K+1; ++i)
			{
				if(s->Vertices[i] == v)
				{
					found = true;
					break;
				}
			}

			if(!found)
				throw MalformedVertexElementConnectionException();
		}
	}

	for(auto s : mData->Elements)
	{
		// Simplex has to set all vertices
		for(Index i = 0; i < K+1; ++i)
		{
			if(!s->Vertices[i])
				throw IncompleteElementException();
			
			auto dist = std::abs((s->Element[i]-s->Vertices[i]->Vertex).magSqr());
			if(dist > std::numeric_limits<decltype(dist)>::epsilon())
				throw IncompleteElementException();
		}

		// No simplices with zero volume
		auto volume = std::abs(s->Element.volume());
		if(volume <= std::numeric_limits<decltype(volume)>::epsilon())
			throw ZeroVolumeSimplexException();
		

		for(Index i = 0; i < K+1; ++i)
		{
			if(!s->Neighbors[i])
				throw MalformedElementNeighborConnectionException();
		}
	}
}

template<typename T, Dimension K>
void Mesh<T,K>::setupBoundaries()
{
	for(auto e : mData->Edges)// N
	{
		if(!e->Elements[0] || !e->Elements[1])
		{
			for(Index i = 0; i < K; ++i)
			{
				NS_ASSERT(e->Vertices[i]);
				e->Vertices[i]->Flags |= MVF_StrongBoundary;
			}
		}
	}
}
NS_END_NAMESPACE