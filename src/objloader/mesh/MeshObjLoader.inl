#ifndef _NS_MESHOBJLOADER_INL
# error MeshObjLoader.inl should only be included by MeshObjLoader.h
#endif

NS_BEGIN_NAMESPACE
namespace Mesh {
	template<typename T>
	Mesh<T,2> MeshObjLoader<T>::loadFile(const std::string& file)
	{		
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;// We ignore materials
		std::string err;

		if (!tinyobj::LoadObj(shapes, materials, err, file.c_str()))
			throw LoadObjErrorException();

		return load(shapes);
	}

	template<typename T>
	Mesh<T,2> MeshObjLoader<T>::loadString(const std::string& str)
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

	template<typename T>
	Mesh<T,2> MeshObjLoader<T>::load(const std::vector<tinyobj::shape_t>& shapes)
	{
		typedef MeshVertex<T,2> MV;
		typedef MeshElement<T,2> ME;

		Mesh<T,2> mesh;

		for (tinyobj::shape_t shape : shapes)
		{
			mesh.reserveVertices(shape.mesh.positions.size() / 3);
			mesh.reserveElements(shape.mesh.indices.size() / 3);

			for (size_t i = 0; i < shape.mesh.positions.size() / 3; ++i)
			{
				MV* v = new MV;
				for(Dimension j = 0; j < 2; ++j)
					v->Vertex[j] = (T)shape.mesh.positions[3*i + j];
				mesh.addVertex(v);
			}

			for (size_t i = 0; i < shape.mesh.indices.size() / 3; ++i)
			{
				ME* s = new ME;
				for (Dimension j = 0; j < 3; ++j)
				{
					Index idx = shape.mesh.indices[3 * i + j];
					s->Vertices[j] = mesh.vertex(idx);
				}

				mesh.addElement(s);
			}

			break;// Only one
		}

		mesh.setupNeighbors();
		return mesh;
	}
}
NS_END_NAMESPACE