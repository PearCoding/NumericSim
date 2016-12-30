#ifndef _NS_HYPERCUBE_INL
# error HyperCube.inl should only be included by HyperCube.h
#endif

NS_BEGIN_NAMESPACE
template<typename T, Dimension K>
Mesh<T,K> HyperCube<T,K>::generate(
	const FixedVector<Dimension,K>& elements,
	const FixedVector<T,K>& size,
	const FixedVector<T,K>& offset)
{
	Mesh<T,K> mesh;

	//TODO
	return mesh;
}

template<typename T>
Mesh<T,2> HyperCube<T,2>::generate(const FixedVector<Dimension,2>& elements,
		const FixedVector<T,2>& size,
		const FixedVector<T,2>& offset)
{
	typedef MeshVertex<T,2> MV;
	typedef MeshElement<T,2> ME;

	Mesh<T,2> mesh;

	if(elements[0] < 1 || elements[1] < 1)
		throw InvalidElementCountException();

	// Reserve container
	mesh.reserveVertices((elements[0]+1)*(elements[1]+1));
	mesh.reserveElements(elements[0]*elements[1]*2);

	// Unit vectors
	FixedVector<T,2> ex;
	ex.fill((T)0);
	ex[0] = size[0]/(T)elements[0];
	FixedVector<T,2> ey;
	ey.fill((T)0);
	ey[1] = size[1]/(T)elements[1];
	
	MV** lastRowVertices = new MV*[elements[1]+1];

	// Generate first row
	for(Index j = 0; j <= elements[1]; ++j)
	{
		MV* v = new MV(offset + ex*(T)j);
		mesh.addVertex(v);
		lastRowVertices[j] = v;
	}

	// Iterate over rows
	for(Index i = 1; i <= elements[0]; ++i)
	{
		const auto py = ey*(T)i;
		MV* lastVertex = new MV(offset + py);
		mesh.addVertex(lastVertex);

		for(Index j = 1; j <= elements[1]; ++j)
		{
			MV* v = new MV(offset + ex*(T)j + py);
			mesh.addVertex(v);

			ME* s1 = new ME();
			s1->Vertices[0] = lastRowVertices[j-1];
			s1->Vertices[1] = lastVertex;
			s1->Vertices[2] = v;
			mesh.addElement(s1);

			ME* s2 = new ME();
			s2->Vertices[0] = lastRowVertices[j-1];
			s2->Vertices[1] = v;
			s2->Vertices[2] = lastRowVertices[j];
			mesh.addElement(s2);

			lastRowVertices[j-1] = lastVertex;
			lastVertex = v;
		}

		lastRowVertices[elements[1]] = lastVertex;
	}

	// Cleanup
	delete[] lastRowVertices;

	mesh.setupNeighbors();
	return mesh;
}
NS_END_NAMESPACE