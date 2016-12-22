#ifndef _NS_MESHOBJLOADER_INL
# error MeshObjLoader.inl should only be included by MeshObjLoader.h
#endif

NS_BEGIN_NAMESPACE
namespace Mesh {
	template<typename T, Dimension N>
	Mesh<T,2,N> MeshObjLoader<T,N>::loadFile(const std::string& file)
	{
		static_assert(N == 2 || N == 3, "Obj loader only supports 2d or 3d data.");
		
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;// We ignore materials
		std::string err;

		if (!tinyobj::LoadObj(shapes, materials, err, file.c_str()))
			throw LoadObjErrorException();

		return load(shapes);
	}

	template<typename T, Dimension N>
	Mesh<T,2,N> MeshObjLoader<T,N>::loadString(const std::string& str)
	{
		static_assert(N == 2 || N == 3, "Obj loader only supports 2d or 3d data.");
		
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

	template<typename T, Dimension N>
	Mesh<T,2,N> MeshObjLoader<T,N>::load(const std::vector<tinyobj::shape_t>& shapes)
	{
		typedef MeshVertex<T,2,N> MV;
		typedef MeshSimplex<T,2,N> MS;

		Mesh<T,2,N> mesh;

		for (tinyobj::shape_t shape : shapes)
		{
			mesh.reserveVertices(shape.mesh.positions.size() / 3);
			mesh.reserveSimplices(shape.mesh.indices.size() / 3);

			for (size_t i = 0; i < shape.mesh.positions.size() / 3; ++i)
			{
				MV* v = new MV;
				for(Dimension j = 0; j < N; ++j)
					v->Vertex[j] = (T)shape.mesh.positions[3*i + j];
				mesh.addVertex(v);
			}

			for (size_t i = 0; i < shape.mesh.indices.size() / 3; ++i)
			{
				MS* s = new MS;
				for (Dimension j = 0; j < 3; ++j)
				{
					Index idx = shape.mesh.indices[3 * i + j];
					s->Vertices[j] = mesh.vertex(idx);
				}

				mesh.addSimplex(s);
			}

			break;// Only one
		}

		mesh.setupNeighbors();
		return mesh;
	}
}
NS_END_NAMESPACE