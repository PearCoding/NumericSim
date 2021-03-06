#ifndef _NS_VTKEXPORTER_INL
# error VTKExporter.inl should only be included by VTKExporter.h
#endif


NS_BEGIN_NAMESPACE

template<typename T, Dimension K>
template<typename V>
void VTKExporter<T,K>::write(const std::string& path,
	const Mesh<T,K>& mesh,
	const std::map<std::string, V*>& pointData,
	const std::map<std::string, V*>& cellData,
	int outputOptions)
{
	static_assert(K >= 1 && K <= 3, "Only 1d, 2d and 3d data can be exported.");

	std::ofstream stream(path.c_str());
	// Header
	stream << "<?xml version=\"1.0\"?>" << std::endl
		<< "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">" << std::endl
		<< "<UnstructuredGrid>" << std::endl
		<< "<Piece NumberOfPoints=\"" << mesh.vertices().size() 
			<<  "\" NumberOfCells=\"" << mesh.elements().size() << "\">" << std::endl;

	// Points
	stream << "<Points>" << std::endl
		<< "<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">" << std::endl;
	
	for (MeshVertex<T,K>* v : mesh.vertices())
	{
		stream << v->Vertex[0] << " "
			<< ((K == 2) ? v->Vertex[1] : 0) << " "
			<< ((K == 3) ? v->Vertex[2] : 0) << std::endl;
	}

	stream << "</DataArray>" << std::endl
		<< "</Points>" << std::endl;

	// Cells
	stream << "<Cells>" <<std::endl;
	stream << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">" << std::endl;
	for (MeshElement<T,K>* elem : mesh.elements())
	{
		for(auto v : elem->DOFVertices)
			stream << v->GlobalIndex << " ";
		stream << std::endl;
	}
	stream << "</DataArray>" << std::endl;

	size_t elemOff = (outputOptions & VOO_IsQuadratic) ? 3 : 2;
	int elemType = (outputOptions & VOO_IsQuadratic) ? 21 : 3;// Line
	if(K == 2)// Triangle
	{
		if(outputOptions & VOO_IsQuadratic)
		{
			elemOff = 6;
			elemType = 22;
		}
		else
		{
			elemOff = 3;
			elemType = 5;
		}
	}
	else if(K == 3)// Tetraeder
	{
		if(outputOptions & VOO_IsQuadratic)
		{
			elemOff = 10;
			elemType = 24;
		}
		else
		{
			elemOff = 4;
			elemType = 10;
		}
	}

	stream << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << std::endl;
	for(Index i = 1; i <= mesh.elements().size(); ++i)
		stream << i*elemOff << " ";
	stream << std::endl;
	stream << "</DataArray>" << std::endl;

	stream << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" << std::endl;
	for(Index i = 0; i < mesh.elements().size(); ++i)
		stream << elemType << " ";
	stream << std::endl;
	
	stream << "</DataArray>" << std::endl;
	stream << "</Cells>" << std::endl;

	// Point data
	stream << "<PointData Scalars=\"" << pointData.begin()->first << "\">" << std::endl;
	for(auto res : pointData)
	{
		stream << "<DataArray Name=\"" << res.first << "\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for(Index i = 0; i < res.second->size(); ++i)
			stream << res.second->at(i) << " ";
		stream << std::endl << "</DataArray>" << std::endl;
	}

	if(outputOptions & VOO_VertexBoundaryLabel)
	{
		stream << "<DataArray Name=\"BoundaryLabel\" type=\"UInt8\" format=\"ascii\">" << std::endl;
		for (MeshVertex<T,K>* v : mesh.vertices())
			stream << ((v->Flags & MVF_StrongBoundary) ? 1 : 0) << " ";
		stream << std::endl << "</DataArray>" << std::endl;
	}

	if(outputOptions & VOO_VertexImplicitLabel)
	{
		stream << "<DataArray Name=\"ImplicitLabel\" type=\"UInt8\" format=\"ascii\">" << std::endl;
		for (MeshVertex<T,K>* v : mesh.vertices())
			stream << ((v->Flags & MVF_Implicit) ? 1 : 0) << " ";
		stream << std::endl << "</DataArray>" << std::endl;
	}
	stream << "</PointData>" << std::endl;

	// Cell data
	stream << "<CellData>" << std::endl;
	for(auto res : cellData)
	{
		stream << "<DataArray Name=\"" << res.first << "\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for(Index i = 0; i < res.second->size(); ++i)
			stream << res.second->at(i) << " ";
		stream << std::endl << "</DataArray>" << std::endl;
	}

	if(outputOptions & VOO_ElementDeterminant)
	{
		stream << "<DataArray Name=\"ElementDeterminant\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for (MeshElement<T,K>* elem : mesh.elements())
			stream << elem->Element.determinant() << " ";
		stream << std::endl;
		stream << "</DataArray>" << std::endl;
	}

	if(outputOptions & VOO_ElementMatrix)
	{
		stream << "<DataArray Name=\"ElementMatrix\" type=\"Float32\" NumberOfComponents=\"" 
			<< (K*K) << "\" format=\"ascii\">" << std::endl;
		for (MeshElement<T,K>* elem : mesh.elements())
		{
			const auto M = elem->Element.matrix();
			for(const auto& v : M)
				stream << v << " ";
			stream << std::endl;
		}
		stream << "</DataArray>" << std::endl;
	}

	if(outputOptions & VOO_ElementGradient)
	{
		stream << "<DataArray Name=\"ElementGradient\" type=\"Float32\" NumberOfComponents=\"" 
			<< (K*K) << "\" format=\"ascii\">" << std::endl;
		for (MeshElement<T,K>* elem : mesh.elements())
		{
			const auto M = elem->Element.gradient(0);
			for(const auto& v : M)
				stream << v << " ";
			stream << std::endl;
		}
		stream << "</DataArray>" << std::endl;
	}
	stream << "</CellData>" << std::endl;

	// End
	stream << "</Piece>" << std::endl;
	stream << "</UnstructuredGrid>" << std::endl;
	stream << "</VTKFile>" << std::endl;
	
	stream << std::endl;
	stream.close();
}

NS_END_NAMESPACE