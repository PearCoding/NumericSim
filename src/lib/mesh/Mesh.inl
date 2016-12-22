#ifndef _NS_MESH_INL
# error Mesh.inl should only be included by Mesh.h
#endif

NS_BEGIN_NAMESPACE
namespace Mesh {
	template<typename T, Dimension K, Dimension N>
	MeshSimplex<T,K,N>::MeshSimplex()
	{
		for(Index i = 0; i < K+1; ++i)
		{
			Vertices[i] = nullptr;
			Neighbors[i].Pointer = nullptr;
		}
	}

	template<typename T, Dimension K, Dimension N>
	Simplex<T,K,N> MeshSimplex<T,K,N>::toSimplex() const
	{
		Simplex<T,K,N> s;
		for(Index i = 0; i < K+1; ++i)
		{
			NS_ASSERT(Vertices[i]);
			s.Vertices[i] = Vertices[i]->Vertex;
		}

		return s;
	}

	//------------------------------------------------
	template<typename T, Dimension K, Dimension N>
	MeshVertex<T,K,N>::MeshVertex()
	{}

	template<typename T, Dimension K, Dimension N>
	MeshVertex<T,K,N>::MeshVertex(const FixedVector<T,N>& vertex) :
		Vertex(vertex)
	{}

	//------------------------------------------------
	template<typename T, Dimension K, Dimension N>
	Mesh<T,K,N>::Mesh()
	{}

	template<typename T, Dimension K, Dimension N>
	Mesh<T,K,N>::~Mesh()
	{
		clear();
	}
		
	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::clear()
	{
		for(auto ptr : mVertices)
			delete ptr;
		mVertices.clear();
		
		for(auto ptr : mSimplices)
			delete ptr;
		mSimplices.clear();
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::reserveVertices(size_t count)
	{
		NS_ASSERT(count > 0);
		mVertices.reserve(count);
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::addVertex(MeshVertex<T,K,N>* vertex)
	{
		NS_ASSERT(vertex);
		mVertices.push_back(vertex);
	}
	
	template<typename T, Dimension K, Dimension N>
	MeshVertex<T,K,N>* Mesh<T,K,N>::vertex(Index i) const
	{
		return mVertices[i];
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::setVertex(Index i, MeshVertex<T,K,N>* vertex)
	{
		mVertices[i] = vertex;
	}

	template<typename T, Dimension K, Dimension N>
	const typename Mesh<T,K,N>::MeshVertexList& Mesh<T,K,N>::vertices() const
	{
		return mVertices;
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::reserveSimplices(size_t count)
	{
		NS_ASSERT(count > 0);
		mSimplices.reserve(count);
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::addSimplex(MeshSimplex<T,K,N>* simplex)
	{
		NS_ASSERT(simplex);

		mSimplices.push_back(simplex);

		for(Index i = 0; i < K+1; ++i)
			simplex->Vertices[i]->Simplices.push_back(simplex);
	}
	
	template<typename T, Dimension K, Dimension N>
	MeshSimplex<T,K,N>* Mesh<T,K,N>::simplex(Index i) const
	{
		return mSimplices[i];
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::setSimplex(Index i, MeshSimplex<T,K,N>* simplex)
	{
		mSimplices[i] = simplex;
	}

	template<typename T, Dimension K, Dimension N>
	const typename Mesh<T,K,N>::MeshSimplexList& Mesh<T,K,N>::simplices() const
	{
		return mSimplices;
	}

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::setupNeighbors()
	{
		for(const auto& S : mSimplices)
		{
			size_t nextEntry = 0;

			for(const auto& other : mSimplices)
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

	template<typename T, Dimension K, Dimension N>
	void Mesh<T,K,N>::validate() const throw(MeshException)
	{
		for(auto v : mVertices)
		{
			// Every vertex has to be connected to atleast one simplex
			if(v->Simplices.empty())
				throw IsolatedVertexException();

			// Every connected simplex has to contain the vertex itself (dev)
			for(auto s : v->Simplices)
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
					throw MalformedVertexSimplexConnectionException();
			}
		}

		for(auto s : mSimplices)
		{
			// Simplex has to set all vertices
			for(Index i = 0; i < K+1; ++i)
			{
				if(!s->Vertices[i])
					throw IncompleteSimplexException();
			}

			// No simplices with zero volume
			auto volume = std::abs(s->toSimplex().volume());
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
						throw SimplexNeighborVertexException();

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
						throw MalformedSimplexNeighborConnectionException();
				}
			}
		}
	}
}
NS_END_NAMESPACE