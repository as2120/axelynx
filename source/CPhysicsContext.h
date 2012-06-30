#pragma once

#include <axelynx/PhysicsContext.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletSoftBody\btSoftBody.h>
#include <BulletSoftBody\btSoftRigidDynamicsWorld.h>

class CBulletPhysicsContext : public axelynx::PhysicsContext
{
	btBroadphaseInterface* broadphase_;
 
    btDefaultCollisionConfiguration* collisionConfiguration_;
    btCollisionDispatcher* dispatcher_;
 
    btSequentialImpulseConstraintSolver* solver_;
	btSoftBodySolver *softbodySolver_;
public:
	CBulletPhysicsContext();
	~CBulletPhysicsContext();

	
	btSoftRigidDynamicsWorld * CreateWorld();

	virtual axelynx::Shape* AddInfinityPlane();
	virtual axelynx::Shape* AddSphere(float radius);
	virtual axelynx::Shape* AddBox(const axelynx::vec3& dimesions);
};