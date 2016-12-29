#ifndef _NS_VTKEXPORTER_INL
# error VTKExporter.inl should only be included by VTKExporter.h
#endif


NS_BEGIN_NAMESPACE

namespace Export
{
	template<typename T, Dimension K>
	template<typename V>
	void VTKExporter<T,K>::write(const std::string& path,
		const Mesh::Mesh<T,K>& mesh, const V& result, V* error)
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
		
		for (Mesh::MeshVertex<T,K>* v : mesh.vertices())
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
		for (Mesh::MeshElement<T,K>* elem : mesh.elements())
		{
			for(Index i = 0; i < K+1; ++i)
				stream << elem->Vertices[i]->GlobalIndex << " ";
			stream << std::endl;
		}
		stream << "</DataArray>" << std::endl;

		stream << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">" << std::endl;
		for(Index i = 1; i <= mesh.elements().size(); ++i)
			stream << i*(K+1) << " " << std::endl;
		stream << std::endl;
		stream << "</DataArray>" << std::endl;

		stream << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">" << std::endl;
		int type = 3;// Line
		if(K == 2)
			type = 5;// Triangle
		else if(K == 3)
			type = 10;// Tetraeder
		for(Index i = 0; i < mesh.elements().size(); ++i)
			stream << type << " ";
		stream << std::endl;
		
		stream << "</DataArray>" << std::endl;
		stream << "</Cells>" << std::endl;
	
		// Point data
		stream << "<PointData Scalars=\"Result\">" << std::endl;
		stream << "<DataArray Name=\"Result\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for(Index i = 0; i < result.size(); ++i)
			stream << result.at(i) << std::endl;
		stream << "</DataArray>" << std::endl;
		
		if(error)
		{
			stream << "<DataArray Name=\"Error\" type=\"Float32\" format=\"ascii\">" << std::endl;
			for(Index i = 0; i < error->size(); ++i)
				stream << error->at(i) << std::endl;
			stream << "</DataArray>" << std::endl;
		}		
		stream << "</PointData>" << std::endl;

		// Cell data
		stream << "<CellData />" << std::endl;

		// End
		stream << "</Piece>" << std::endl;
		stream << "</UnstructuredGrid>" << std::endl;
		stream << "</VTKFile>" << std::endl;
		
		stream << std::endl;
		stream.close();
	}
}

NS_END_NAMESPACE