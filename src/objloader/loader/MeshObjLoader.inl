#ifndef _NS_MESHOBJLOADER_INL
# error MeshObjLoader.inl should only be included by MeshObjLoader.h
#endif

NS_BEGIN_NAMESPACE

template<typename T, Index yI, Index xI>
Mesh<T,2> MeshObjLoader<T,yI,xI>::loadFile(const std::string& file)
{		
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;// We ignore materials
	std::string err;

	if (!tinyobj::LoadObj(shapes, materials, err, file.c_str()))
		throw LoadObjErrorException();

	return load(shapes);
}

template<typename T, Index yI, Index xI>
Mesh<T,2> MeshObjLoader<T,yI,xI>::loadString(const std::string& str)
{		
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;// We ignore materials
	std::string err;
	std::istringstream stream(str);
	auto reader = tinyobj::MaterialFileReader("");
	
	if (!tinyobj::LoadObj(shapes, materials, err,
		stream, reader))
		throw LoadObjErrorException();

	return load(shapes);
}

template<typename T, Index yI, Index xI>
Mesh<T,2> MeshObjLoader<T,yI,xI>::load(const std::vector<tinyobj::shape_t>& shapes)
{
	typedef MeshVertex<T,2> MV;
	typedef MeshElement<T,2> ME;

	Mesh<T,2> mesh;

	for (tinyobj::shape_t shape : shapes)
	{
		const size_t vertexCount = shape.mesh.positions.size() / 3;
		const size_t elementCount = shape.mesh.indices.size() / 3;

		mesh.reserveVertices(vertexCount);
		mesh.reserveElements(elementCount);

		for (size_t i = 0; i < vertexCount; ++i)
		{
			MV* v = new MV();
			v->Vertex[0] = (T)shape.mesh.positions[3*i + xI];
			v->Vertex[1] = (T)shape.mesh.positions[3*i + yI];
			mesh.addVertex(v);
		}

		for (size_t i = 0; i < elementCount; ++i)
		{
			ME* s = new ME();
			for (Dimension j = 0; j < 3; ++j)
			{
				Index idx = shape.mesh.indices[3 * i + j];
				if(idx >= vertexCount)
					throw LoadObjErrorException();
				
				s->Vertices[j] = mesh.vertex(idx);
			}

			mesh.addElement(s);
		}

		break;// Only one
	}

	mesh.setupNeighbors();
	return mesh;
}

NS_END_NAMESPACE