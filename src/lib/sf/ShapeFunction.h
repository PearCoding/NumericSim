#pragma once

#include "Config.h"

NS_BEGIN_NAMESPACE

namespace SF
{
	template<class ShapeFunctionPolicy>
	class ShapeFunction : public ShapeFunctionPolicy
	{
	public:

	};
}

NS_END_NAMESPACE


#define _NS_SHAPEFUNCTION_INL
# include "ShapeFunction.inl"
#undef _NS_SHAPEFUNCTION_INL