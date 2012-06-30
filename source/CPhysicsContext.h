#pragma once

#include <axelynx/PhysicsContext.h>
#include <btBulletDynamicsCommon.h>

class CBulletPhysicsContext : public axelynx::PhysicsContext
{
	btBroadphaseInterface* broadphase_;
 
    btDefaultCollisionConfiguration* collisionConfiguration_;
    btCollisionDispatcher* dispatcher_;
 
    btSequentialImpulseConstraintSolver* solver_;
public:
	CBulletPhysicsContext();
	~CBulletPhysicsContext();

	
	btDiscreteDynamicsWorld * CreateWorld();

	virtual axelynx::Shape* AddInfinityPlane();
	virtual axelynx::Shape* AddSphere(float radius);
	virtual axelynx::Shape* AddBox(const axelynx::vec3& dimesions);
};