#pragma once

#include "mesh/Mesh.h"

#include "../external/tiny_obj_loader.h"

#include <sstream>

NS_BEGIN_NAMESPACE

NS_DECLARE_EXCEPTION_GROUP(ObjLoader, Mesh);
NS_DECLARE_EXCEPTION(LoadObjError, ObjLoader, "Error while loading the obj file.");

namespace Mesh
{
	template<typename T>
	class MeshObjLoader
	{
	public:
		static Mesh<T,2> loadFile(const std::string& file);
		static Mesh<T,2> loadString(const std::string& str);

	private:
		static Mesh<T,2> load(const std::vector<tinyobj::shape_t>& shapes);
	};
}

NS_END_NAMESPACE


#define _NS_MESHOBJLOADER_INL
# include "MeshObjLoader.inl"
#undef _NS_MESHOBJLOADER_INL