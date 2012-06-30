#pragma once

#include <axelynx/Body.h>
#include "CShape.h"
#include <vector>
#include <btBulletDynamicsCommon.h>

class CBody : public axelynx::Body
{
	CShape *shape_;
	std::vector<axelynx::Pivot*> linked_pivots_;

	btDefaultMotionState* moution_state_;
	btRigidBody* rigid_body_;
public:
	CBody(CShape *shape, axelynx::vec3 position, axelynx::quat orientation, float mass);

	virtual int AddPivot(axelynx::Pivot *pivot);
	virtual axelynx::Pivot* GetPivot(int index) const;
	virtual int CountPivots() const;
	virtual int ClearPivots();
	virtual void RemovePivot(int index);
	virtual void RemovePivot(axelynx::Pivot* pivot);

	virtual axelynx::vec3 GetPosition();
	virtual axelynx::quat GetOrientation();

	virtual void AddImpulse(const axelynx::vec3& force);
	virtual void AddForce(const axelynx::vec3& force);

	btRigidBody* GetRigidBody()
	{
		return rigid_body_;
	}
	virtual ~CBody();
};
