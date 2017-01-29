#pragma once

#include "mesh/Mesh.h"

#include <string>
#include <fstream>
#include <map>

NS_BEGIN_NAMESPACE

enum VTKOutputOptions
{
	VOO_ElementDeterminant = 0x1,
	VOO_ElementMatrix = 0x2,
	VOO_ElementGradient = 0x4,
	VOO_VertexBoundaryLabel = 0x8,
	VOO_VertexImplicitLabel = 0x10,

	VOO_IsQuadratic = 0x80
};

template<typename T, Dimension K>
class VTKExporter
{
public:
	template<typename V>
	static void write(const std::string& path,
		const Mesh<T,K>& mesh,
		const std::map<std::string, V*>& pointData,
		const std::map<std::string, V*>& cellData,
		int outputOptions = 0);
};

NS_END_NAMESPACE


#define _NS_VTKEXPORTER_INL
# include "VTKExporter.inl"
#undef _NS_VTKEXPORTER_INL