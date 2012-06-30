#include "CPhysicsContext.h"	

#include "CShape.h"

CBulletPhysicsContext::CBulletPhysicsContext()
{
	broadphase_ = new btDbvtBroadphase();
 
    // Set up the collision configuration and dispatcher
    collisionConfiguration_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
 
    // The actual physics solver
    solver_ = new btSequentialImpulseConstraintSolver;
}


CBulletPhysicsContext::~CBulletPhysicsContext()
{
	delete solver_;
    delete dispatcher_;
    delete collisionConfiguration_;
    delete broadphase_;
}

btDiscreteDynamicsWorld* CBulletPhysicsContext::CreateWorld()
{
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,collisionConfiguration_);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

	return dynamicsWorld;
}


axelynx::Shape* CBulletPhysicsContext::AddInfinityPlane()
{
	btCollisionShape* shape = new btStaticPlaneShape(btVector3(0,1,0),1);
	CShape *cshape = new CShape(shape);
	
	return cshape;
}

axelynx::Shape* CBulletPhysicsContext::AddSphere(float radius)
{
	btCollisionShape* shape = new btSphereShape(radius);
	CShape *cshape = new CShape(shape);
	
	return cshape;
}

axelynx::Shape* CBulletPhysicsContext::AddBox(const axelynx::vec3& dimesions)
{	
	btCollisionShape* shape = new btBoxShape(btVector3(dimesions.x,dimesions.y,dimesions.z));

	CShape *cshape = new CShape(shape);
	
	return cshape;
}