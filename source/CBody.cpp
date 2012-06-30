#include "CBody.h"


CBody::CBody(CShape *shape, axelynx::vec3 position, axelynx::quat orientation, float mass)
{
	shape_ = shape;

	moution_state_ =
                new btDefaultMotionState(btTransform(btQuaternion(orientation.x,orientation.y,orientation.z,orientation.w),btVector3(position.x,position.y,position.z)));

	btVector3 inertia = btVector3(0,0,0);

	if(mass>0)
	{
		shape->GetShape()->calculateLocalInertia(mass,inertia);
	}

	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(mass,moution_state_,shape->GetShape(),inertia);
       
	rigid_body_ = new btRigidBody(groundRigidBodyCI);
}

CBody::~CBody()
{
	delete rigid_body_;
	delete moution_state_;
}


int CBody::AddPivot(axelynx::Pivot *pivot)
{
	linked_pivots_.push_back(pivot);

	return linked_pivots_.size() - 1;
}

axelynx::Pivot* CBody::GetPivot(int index) const
{
	return linked_pivots_[index];
}

int CBody::CountPivots() const
{
	return linked_pivots_.size();
}

int CBody::ClearPivots()
{
	int size = CountPivots();

	linked_pivots_.clear();

	return size;
}

void CBody::RemovePivot(int index)
{
	//TODO
}

void CBody::RemovePivot(axelynx::Pivot* pivot)
{
	//TODO
}

axelynx::vec3 CBody::GetPosition()
{
	btTransform trans;
	rigid_body_->getMotionState()->getWorldTransform(trans);

	btVector3 pos = trans.getOrigin();

	return axelynx::vec3(pos.x(),pos.y(),pos.z());
}

axelynx::quat CBody::GetOrientation()
{
	btTransform trans;
	rigid_body_->getMotionState()->getWorldTransform(trans);

	btQuaternion orient = trans.getRotation();

	return axelynx::quat(orient.x(),orient.y(),orient.z(),orient.w());
}

void CBody::AddImpulse(const axelynx::vec3& force)
{
	rigid_body_->applyCentralImpulse(btVector3(force.x,force.y,force.z));
}

void CBody::AddForce(const axelynx::vec3& force)
{
	rigid_body_->applyCentralForce(btVector3(force.x,force.y,force.z));
}