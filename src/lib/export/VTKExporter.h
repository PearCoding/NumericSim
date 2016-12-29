#pragma once

#include "mesh/Mesh.h"

#include <string>
#include <fstream>

NS_BEGIN_NAMESPACE

namespace Export
{
	template<typename T, Dimension K>
	class VTKExporter
	{
	public:
	    template<typename V>
        static void write(const std::string& path,
            const Mesh::Mesh<T,K>& mesh, const V& result, V* error = nullptr);
	};
}

NS_END_NAMESPACE


#define _NS_VTKEXPORTER_INL
# include "VTKExporter.inl"
#undef _NS_VTKEXPORTER_INL