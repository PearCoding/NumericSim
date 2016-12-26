#ifndef _NS_MESH_INL
# error Mesh.inl should only be included by Mesh.h
#endif

NS_BEGIN_NAMESPACE
namespace Mesh {
	template<typename T, Dimension K>
	MeshElement<T,K>::MeshElement()
	{
		for(Index i = 0; i < K+1; ++i)
		{
			Vertices[i] = nullptr;
			Neighbors[i].Pointer = nullptr;
		}
	}

	//------------------------------------------------
	template<typename T, Dimension K>
	MeshVertex<T,K>::MeshVertex()
	{}

	template<typename T, Dimension K>
	MeshVertex<T,K>::MeshVertex(const FixedVector<T,K>& vertex) :
		Vertex(vertex)
	{}

	//------------------------------------------------
	template<typename T, Dimension K>
	Mesh<T,K>::Mesh()
	{}

	template<typename T, Dimension K>
	Mesh<T,K>::~Mesh()
	{
		clear();
	}
		
	template<typename T, Dimension K>
	void Mesh<T,K>::clear()
	{
		for(auto ptr : mVertices)
			delete ptr;
		mVertices.clear();
		
		for(auto ptr : mElements)
			delete ptr;
		mElements.clear();
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::reserveVertices(size_t count)
	{
		NS_ASSERT(count > 0);
		mVertices.reserve(count);
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::addVertex(MeshVertex<T,K>* vertex)
	{
		NS_ASSERT(vertex);
		vertex->GlobalIndex = mVertices.size();
		mVertices.push_back(vertex);
	}
	
	template<typename T, Dimension K>
	MeshVertex<T,K>* Mesh<T,K>::vertex(Index i) const
	{
		return mVertices[i];
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::setVertex(Index i, MeshVertex<T,K>* vertex)
	{
		mVertices[i] = vertex;
	}

	template<typename T, Dimension K>
	const typename Mesh<T,K>::MeshVertexList& Mesh<T,K>::vertices() const
	{
		return mVertices;
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::reserveElements(size_t count)
	{
		NS_ASSERT(count > 0);
		mElements.reserve(count);
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::addElement(MeshElement<T,K>* element)
	{
		NS_ASSERT(element);

		mElements.push_back(element);

		for(Index i = 0; i < K+1; ++i)
		{
			element->Element[i] = element->Vertices[i]->Vertex;
			element->Vertices[i]->Elements.push_back(element);
		}
	}
	
	template<typename T, Dimension K>
	MeshElement<T,K>* Mesh<T,K>::element(Index i) const
	{
		return mElements[i];
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::setElement(Index i, MeshElement<T,K>* element)
	{
		mElements[i] = element;
	}

	template<typename T, Dimension K>
	const typename Mesh<T,K>::MeshElementList& Mesh<T,K>::elements() const
	{
		return mElements;
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::setupNeighbors()
	{
		for(const auto& S : mElements)
		{
			size_t nextEntry = 0;

			for(const auto& other : mElements)
			{
				if(nextEntry >= K+1)// Already full
					break;
				
				// If already neighbor (can happen due to symmetry), skip this entry
				bool alreadyNeighbor = false;
				for(Index i = 0; i < K+1; ++i)
				{
					if(S->Neighbors[i].Pointer == other)
					{
						alreadyNeighbor = true;
						break;
					}
				}

				if(alreadyNeighbor)
					continue;

				// Check if they share some vertices
				size_t sharedVertices = 0;
				for(Index i = 0; i < K+1; ++i)
				{
					const auto& v = S->Vertices[i];
					bool found = false;
					for(Index j = 0; j < K+1; ++j)
					{
						if(v == other->Vertices[j])
						{
							found = true;
							break;
						}
					}

					if(found)
						sharedVertices++;
				}

				// K+1 would be the simplex itself, and everything less than K is not sharing
				if(sharedVertices == K)
				{
					size_t otherEntry = K+1;
					for(otherEntry = 0; otherEntry < K+1; ++otherEntry)
					{
						if(!other->Neighbors[otherEntry].Pointer)
							break;
					}

					if(otherEntry >= K+1)// Something in the mesh generation failed
						throw TooManySharedFacesException();

					S->Neighbors[nextEntry].Pointer = other;
					other->Neighbors[otherEntry].Pointer = S;

					size_t vertexEntry = 0;
					for(Index i = 0; i < K+1; ++i)
					{
						NS_ASSERT(vertexEntry < K+1);

						const auto& v = S->Vertices[i];
						for(Index j = 0; j < K+1; ++j)
						{
							if(v == other->Vertices[j])
							{
								S->Neighbors[nextEntry].SharedVertices[vertexEntry] = v;
								other->Neighbors[otherEntry].SharedVertices[vertexEntry] = v;
								vertexEntry++;
							}
						}
					}

					nextEntry++;
				}
			}
		}
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::prepare()
	{
		for(auto s : mElements)
			s->Element.prepare();
	}

	template<typename T, Dimension K>
	void Mesh<T,K>::validate() const throw(MeshException)
	{
		Index id = 0;
		for(auto v : mVertices)
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

		for(auto s : mElements)
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
				const auto& neighbor = s->Neighbors[i];

				if(!neighbor.Pointer)
					continue;

				for(Index j = 0; j < K; ++j)
				{
					auto v = neighbor.SharedVertices[j];

					if(!v)
						throw ElementNeighborVertexException();

					bool found = false;
					for(Index k = 0; k < K+1; ++k)
					{
						if(s->Vertices[k] == v)
						{
							found = true;
							break;
						}
					}

					if(!found)
						throw MalformedElementNeighborConnectionException();
				}
			}
		}
	}
}
NS_END_NAMESPACE