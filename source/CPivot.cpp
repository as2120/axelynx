

#include "CPivot.h"
#include "CScene.h"
#include "axelynx/utils/axstring.h"

#include <iostream>
//interface
AXELYNX_API axelynx::Pivot::Pivot()
{
	pimpl = new axelynx::Pivot::CPivot();
}

axelynx::Pivot* axelynx::Pivot::SetParent(Pivot * parent)
{
	pimpl->SetParent(parent);
	parent->pimpl->AddChild(this);

	return this;
}

axelynx::Pivot* axelynx::Pivot::GetParent() const
{
	return pimpl->GetParent();
}

bool axelynx::Pivot::isMustRecalcTM() const
{
	return pimpl->isMustRecalcTM();
}

int axelynx::Pivot::AddChild(axelynx::Pivot * child)
{
	child->pimpl->SetParent(this);
	return pimpl->AddChild(child);
}

std::list<axelynx::Pivot*>* axelynx::Pivot::ChildsList()
{
	return pimpl->ChildsList();
}

std::list<const axelynx::Pivot*>* axelynx::Pivot::ChildsList() const
{
	return const_cast<const axelynx::Pivot::CPivot*>(pimpl)->ChildsList();
}

axelynx::Pivot* axelynx::Pivot::SetPosition(const axelynx::vec3& position)
{
	pimpl->SetPosition(position);
	return this;
}

axelynx::Pivot* axelynx::Pivot::SetScale(const axelynx::vec3& scale)
{
	pimpl->SetScale(scale);
	return this;
}

axelynx::Pivot* axelynx::Pivot::SetOrientation(const axelynx::quat& orient)
{
	pimpl->SetOrientation(orient);
	return this;
}

axelynx::Pivot* axelynx::Pivot::SetRotate(const axelynx::vec3& angles)
{
	pimpl->SetRotate(angles);
	return this;
}

const axelynx::vec3& axelynx::Pivot::GetPosition(bool global,bool immediate) const
{
	return pimpl->GetPosition(global,immediate);
}

const axelynx::vec3& axelynx::Pivot::GetScale(bool global) const
{
	return pimpl->GetScale(global);
}

const axelynx::quat& axelynx::Pivot::GetOrientation(bool global) const
{
	return pimpl->GetOrientation(global);
}

const axelynx::mat3& axelynx::Pivot::GetOrientationMatrix() const
{
	return pimpl->GetOrientationMatrix();
}

const axelynx::mat4& axelynx::Pivot::GetTransformMatrix(bool immediate) const
{
	return pimpl->GetTransformMatrix(immediate);
}

const axelynx::mat4& axelynx::Pivot::GetLocalMatrix() const
{
	return pimpl->GetLocalMatrix();
}

axelynx::Pivot* axelynx::Pivot::Move(const axelynx::vec3& translate)
{
	pimpl->Move(translate);
	return this;
}

axelynx::Pivot* axelynx::Pivot::Move(float value)
{
	pimpl->Move(value);
	return this;
}

axelynx::Pivot* axelynx::Pivot::MoveX(float value)
{
	pimpl->MoveX(value);
	return this;
}

axelynx::Pivot* axelynx::Pivot::MoveY(float value)
{
	pimpl->MoveY(value);
	return this;
}

axelynx::Pivot* axelynx::Pivot::MoveZ(float value)
{
	pimpl->MoveZ(value);
	return this;
}

axelynx::Pivot* axelynx::Pivot::Turn(const axelynx::vec3& angles)
{
	pimpl->Turn(angles);
	return this;
}

axelynx::Pivot* axelynx::Pivot::Turn(const axelynx::quat& orient)
{
	pimpl->Turn(orient);
	return this;
}

axelynx::Pivot* axelynx::Pivot::PointOn(const axelynx::vec3& point)
{
	pimpl->PointOn(point);
	return this;
}

axelynx::Pivot* axelynx::Pivot::Translate(const axelynx::vec3& translate)
{
	pimpl->Translate(translate);
	return this;
}

axelynx::Pivot::~Pivot()
{
	delete pimpl;
};

void axelynx::Pivot::SetBody(axelynx::Body* body)
{
	pimpl->SetBody(body);
}

axelynx::Body* axelynx::Pivot::GetBody()
{
	return pimpl->GetBody();
}


//implementation
axelynx::Pivot::CPivot::CPivot()
	:position_(0),orientation_(),scale_(1.0f),parent_(0),childs_(0),scene_frame_(-1),body_(0),recalc_(true)
{
}

void axelynx::Pivot::CPivot::SetParent(axelynx::Pivot * parent)
{
	parent_ = parent;
}

axelynx::Pivot* axelynx::Pivot::CPivot::GetParent() const
{
	return parent_;
}

int axelynx::Pivot::CPivot::AddChild(axelynx::Pivot * child)
{
	if(!childs_)
	{
		childs_ = new std::list<Pivot*>();
	}

	childs_->push_back(child);
	
	return 1;
}

std::list<axelynx::Pivot*>* axelynx::Pivot::CPivot::ChildsList()
{
	return childs_;
}

std::list<const axelynx::Pivot*>* axelynx::Pivot::CPivot::ChildsList() const
{
	return (std::list<const axelynx::Pivot*>*)childs_;
}

void axelynx::Pivot::CPivot::SetPosition(const axelynx::vec3& position)
{
	position_ = position;
	recalc_ = true;
}

void axelynx::Pivot::CPivot::SetScale(const axelynx::vec3& scale)
{
	scale_ = scale;
	recalc_ = true;
}

void axelynx::Pivot::CPivot::SetOrientation(const axelynx::quat& orient)
{
	orientation_ = orient;
	recalc_ = true;
}

void axelynx::Pivot::CPivot::SetRotate(const axelynx::vec3& angles)
{
}

const axelynx::vec3& axelynx::Pivot::CPivot::GetPosition(bool global,bool immediate) const
{
	if(global)
	{
		static axelynx::vec3 sresult;
		const axelynx::mat4& ctransform = GetTransformMatrix(immediate);
		sresult = vec3(ctransform[12]
			,ctransform[13]
			,ctransform[14]);
		return sresult;
	}
	else
		return position_;
}

const axelynx::vec3& axelynx::Pivot::CPivot::GetScale(bool global) const
{
	return scale_;
}

const axelynx::quat& axelynx::Pivot::CPivot::GetOrientation(bool global) const
{
	return orientation_;
}

const axelynx::mat3& axelynx::Pivot::CPivot::GetOrientationMatrix() const
{
	if(recalc_)
	{
		rotate_ = orientation_.ToMat3();
	}

	return rotate_;
}

const axelynx::mat4& axelynx::Pivot::CPivot::GetTransformMatrix(bool immediate) const
{
	if(immediate)
	{
		if(parent_)
			transform_ = GetLocalMatrix() * parent_->GetTransformMatrix(true);
		else
			transform_ = GetLocalMatrix();
	}
	else
	{
		int frame = CScene::GetFrame();
		if(scene_frame_ != frame)
		{
			if(parent_)
				transform_ = GetLocalMatrix() * parent_->GetTransformMatrix(false);
			else
				transform_ = GetLocalMatrix();

			scene_frame_ = frame;
		}
	}

	return transform_;
}

const axelynx::mat4& axelynx::Pivot::CPivot::GetLocalMatrix() const
{
	if(recalc_)
	{
		CalcTransform();
	}

	return local_;
}

void axelynx::Pivot::CPivot::CalcTransform() const
{
	//identity
	//local_ = axelynx::mat4::Make(orientation_,position_,scale_);
	axelynx::mat4::Make(local_,orientation_,position_,scale_);

	recalc_ = false;
}


void axelynx::Pivot::CPivot::Move(const vec3& translate)
{
	axelynx::mat3 m = orientation_.ToMat3();

	vec3 tr = m * translate;
	position_ += tr;

	recalc_ = true;
}
void axelynx::Pivot::CPivot::Move(float value)
{
	vec3 tr = orientation_.ztransform(value);

	position_ += tr;

	recalc_ = true;
}

void axelynx::Pivot::CPivot::MoveX(float value)
{
}

void axelynx::Pivot::CPivot::MoveY(float value)
{
}

void axelynx::Pivot::CPivot::MoveZ(float value)
{
}

void axelynx::Pivot::CPivot::Turn(const vec3& angles)
{
	axelynx::quat q(angles);
	Turn(q);
}

void axelynx::Pivot::CPivot::Turn(const quat& orient)
{
	orientation_ = orientation_ * orient;
	recalc_ = true;
}

void axelynx::Pivot::CPivot::PointOn(const vec3& point)
{
	using namespace axelynx;
	
	vec3 dir = point - GetPosition(false);

	dir.normalize();
	quat q(dir,1);

	std::cout<<"x:"<<q.x<<"y:"<<q.y<<"z:"<<q.z<<"w:"<<q.w<<std::endl;
	SetOrientation(q);
}

void axelynx::Pivot::CPivot::Translate(const axelynx::vec3& translate)
{
	position_ += translate;
	recalc_ = true;
}

axelynx::Pivot::CPivot::~CPivot()
{
	if(childs_)
	{
		if(!childs_->empty())
		{
			auto ci = childs_->begin(), ei = childs_->end();

			for(;ci!=ei;++ci)
			{
				delete (*ci);
			}
		}
	}

}

int axelynx::Pivot::GetBinarySize() const
{
	int size = 0;
	
	size += sizeof(axelynx::vec3); //position
	size += sizeof(axelynx::vec4); //orientation
	size += sizeof(axelynx::vec3); //scale

	size += sizeof(int);
	if(ChildsList() == 0)
	{
		auto ci = ChildsList()->begin();
		auto ei = ChildsList()->end();

		for(;ci!=ei;++ci)
		{
			size += (*ci)->GetBinarySize() + sizeof(int);
		}
	}

	return size;
}

int axelynx::Pivot::WriteBinary(axelynx::File file) const
{

	return 0;
}

void axelynx::Pivot::CPivot::SetBody(axelynx::Body *body)
{
	body_ = body;
}

axelynx::Body * axelynx::Pivot::CPivot::GetBody()
{
	return body_;
}