#pragma once

#include "mesh/Mesh.h"

#include <sstream>
#include <fstream>
#include <streambuf>

NS_BEGIN_NAMESPACE

NS_DECLARE_EXCEPTION_GROUP(TriangleLoader, Mesh);
NS_DECLARE_EXCEPTION(LoadTriangleNodeError, TriangleLoader, "Error while loading the .node file.");
NS_DECLARE_EXCEPTION(LoadTriangleElementError, TriangleLoader, "Error while loading the .ele file.");

template<typename T>
class MeshTriangleLoader
{
public:
	static Mesh<T,2> loadFile(const std::string& nodeFile, const std::string& eleFile);
	static Mesh<T,2> loadString(const std::string& nodeStr, const std::string& eleStr);

private:
	static void setupNode(Mesh<T,2>& mesh, const std::string& nodeStr);
	static void setupElement(Mesh<T,2>& mesh, const std::string& eleStr);
};

NS_END_NAMESPACE

#define _NS_MESHTRIANGLELOADER_INL
# include "MeshTriangleLoader.inl"
#undef _NS_MESHTRIANGLELOADER_INL