#ifndef _NS_MESHTRIANGLELOADER_INL
# error MeshTriangleLoader.inl should only be included by MeshTriangleLoader.h
#endif

NS_BEGIN_NAMESPACE

template<typename T>
Mesh<T,2> MeshTriangleLoader<T>::loadFile(const std::string& nodeFile, const std::string& eleFile)
{
	std::ifstream f1(nodeFile);
	std::ifstream f2(eleFile);

	return loadString(
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
	uint32 parseMode = 0;
	uint32 indexShift = 1;
	size_t nextIndex = 0;

	std::istringstream f(nodeStr);
    std::string line;    
    while (std::getline(f, line))
	{
		auto it = line.cbegin();

		skipWS(it, line.cend());
		if(it == line.end())
			continue;

		if(parseMode == 0)//First Line
		{
			size_t nodes = extractFirstNumber<size_t>(it, line.end());

			if(nodes == 0)
				throw LoadTriangleNodeErrorException();

			skipWS(it, line.end());
			if(it == line.end() || extractFirstNumber<int>(it, line.end()) != 2)
				throw LoadTriangleNodeErrorException();
			
			mesh.reserveVertices(nodes);
			// Everything else is ignored!
			parseMode = 1;
		}
		else// Vertices
		{
			size_t index = extractFirstNumber<size_t>(it, line.end());
			if(index == 0)
				indexShift = 0;
			
			if(index-indexShift != nextIndex)
				throw LoadTriangleNodeErrorException();
			nextIndex = index-indexShift + 1;
			
			skipWS(it, line.end());
			if(it == line.end())
				throw LoadTriangleNodeErrorException();
			
			T e1 = extractFirstNumber<T>(it, line.end());

			skipWS(it, line.end());
			if(it == line.end())
				throw LoadTriangleNodeErrorException();

			T e2 = extractFirstNumber<T>(it, line.end());

			// Everything else is ignored

			MeshVertex<T,2>* mv = new MeshVertex<T,2>(FixedVector<T,2>{e1,e2});
			mesh.addVertex(mv);
		}
    }
}

template<typename T>
void MeshTriangleLoader<T>::setupElement(Mesh<T,2>& mesh, const std::string& eleStr)
{
	uint32 parseMode = 0;
	uint32 indexShift = 1;
	size_t nextIndex = 0;

	std::istringstream f(eleStr);
    std::string line;    
    while (std::getline(f, line))
	{
		auto it = line.cbegin();

		skipWS(it, line.cend());
		if(it == line.end())
			continue;

		if(parseMode == 0)//First Line
		{
			size_t nodes = extractFirstNumber<size_t>(it, line.end());

			if(nodes == 0)
				throw LoadTriangleElementErrorException();

			skipWS(it, line.end());
			if(it == line.end() || extractFirstNumber<int>(it, line.end()) != 3)
				throw LoadTriangleElementErrorException();
			
			mesh.reserveVertices(nodes);
			// Everything else is ignored!
			parseMode = 1;
		}
		else// Vertices
		{
			size_t index = extractFirstNumber<size_t>(it, line.end());
			if(index == 0)
				indexShift = 0;
			
			if(index-indexShift != nextIndex)
				throw LoadTriangleNodeErrorException();
			nextIndex = index-indexShift + 1;
			
			skipWS(it, line.end());
			if(it == line.end())
				throw LoadTriangleNodeErrorException();
			
			size_t i1 = extractFirstNumber<size_t>(it, line.end());

			skipWS(it, line.end());
			if(it == line.end())
				throw LoadTriangleNodeErrorException();

			size_t i2 = extractFirstNumber<size_t>(it, line.end());

			skipWS(it, line.end());
			if(it == line.end())
				throw LoadTriangleNodeErrorException();

			size_t i3 = extractFirstNumber<size_t>(it, line.end());

			// Everything else is ignored

			MeshElement<T,2>* me = new MeshElement<T,2>();
			me->Vertices[0] = mesh.vertex(i1-indexShift);
			me->Vertices[1] = mesh.vertex(i2-indexShift);
			me->Vertices[2] = mesh.vertex(i3-indexShift);

			mesh.addElement(me);
		}
    }
}

template<typename T>
void MeshTriangleLoader<T>::skipWS(std::string::const_iterator& it,
	std::string::const_iterator end)
{
	while(it != end)
	{
		if(*it == '#')
		{
			it = end;
			return;
		}
		else if(!isspace(*it))
			return;

		++it;
	}
}

template<typename T>
template<typename T2>
T2 MeshTriangleLoader<T>::extractFirstNumber(std::string::const_iterator& start,
	std::string::const_iterator end)
{
	std::stringstream stream(std::string(start, end));
	T2 ret = 0;
	stream >> ret;
	start += stream.tellg();

	return ret;
}

NS_END_NAMESPACE