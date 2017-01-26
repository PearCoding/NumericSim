#pragma once

#include "nsConfig.h"

NS_BEGIN_NAMESPACE

template<class ShapeFunctionPolicy>
class ShapeFunction : public ShapeFunctionPolicy
{
public:
};

NS_END_NAMESPACE


#define _NS_SHAPEFUNCTION_INL
# include "ShapeFunction.inl"
#undef _NS_SHAPEFUNCTION_INL