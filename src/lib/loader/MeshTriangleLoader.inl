#ifndef _NS_MESHTRIANGLELOADER_INL
# error MeshTriangleLoader.inl should only be included by MeshTriangleLoader.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
Mesh<T,2> MeshTriangleLoader<T>::loadFile(const std::string& nodeFile, const std::string& eleFile)
{
	std::ifstream f1(nodeFile);
	std::ifstream f2(eleFile);

	return loadSring(
		std::string(std::istreambuf_iterator<char>(f1), std::istreambuf_iterator<char>()),
		std::string(std::istreambuf_iterator<char>(f2), std::istreambuf_iterator<char>()));
}

template<typename T>
Mesh<T,2> MeshTriangleLoader<T>::loadString(const std::string& nodeStr, const std::string& eleStr)
{		
	Mesh<T,2> mesh;

	setupNode(mesh, nodeStr);
	setupElement(mesh, eleStr);
	mesh.setupNeighbors();

	return mesh;
}

template<typename T>
void MeshTriangleLoader<T>::setupNode(Mesh<T,2>& mesh, const std::string& nodeStr)
{
	//TODO
}

template<typename T>
void MeshTriangleLoader<T>::setupElement(Mesh<T,2>& mesh, const std::string& eleStr)
{
	//TODO
}

NS_END_NAMESPACE