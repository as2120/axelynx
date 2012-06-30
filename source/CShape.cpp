#include "CShape.h"


CShape::CShape(btCollisionShape *shape)
{
	shape_ = shape;
}

CShape::~CShape()
{
}