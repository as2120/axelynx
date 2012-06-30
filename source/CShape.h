#pragma once

#include <axelynx/Shape.h>

#include <btBulletDynamicsCommon.h>

class CShape : public axelynx::Shape
{
	btCollisionShape *shape_;
public:
	CShape(btCollisionShape *shape);
	virtual ~CShape();

	btCollisionShape * GetShape()
	{
		return shape_;
	}
};
