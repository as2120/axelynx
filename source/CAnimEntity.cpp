

#include "CAnimEntity.h"
#include "gl/axgl.h"
#include "CEarlyZ.h"
#include "CShadowPass.h"
#include "CCanvas.h"
#include <map>
#include "axelynx/Shell.h"
#include <iostream>
#include "CScene.h"

CAnimEntity::CAnimEntity(axelynx::Scene *scene_, const axelynx::AnimGeometry * geom)
	:CEntity(scene_,geom)
{
	frame_ = 0;

	maxframe_ = geom->CountFrames() -  1;
	loop_ = true;
	speedfactor_ = 1.0;
}

void CAnimEntity::SetSpeedFactor(float speedfactor)
{
	speedfactor_ = speedfactor;
}

CAnimEntity::~CAnimEntity()
{
//	CEntity::~CEntity();
}

void CAnimEntity::SetFrame(float frame)
{
	frame_ = frame;
}

bool CAnimEntity::Draw(const axelynx::Camera *camera)
{
	CEntity::PreparePipeline(camera);

	const axelynx::AnimGeometry *animgeom = static_cast<const axelynx::AnimGeometry*>(CEntity::GetGeometry());

	OPENGL_CHECK_FOR_ERRORS();
	animgeom->Draw(frame_);
	OPENGL_CHECK_FOR_ERRORS();
	return true;
}

	const axelynx::AnimGeometry * CAnimEntity::GetGeometry() const
	{
		return static_cast<const axelynx::AnimGeometry*>(CEntity::GetGeometry());
	}

	bool CAnimEntity::Update(float twin)
	{
		frame_ += twin * speedfactor_;

		if(frame_>maxframe_)
		{
			if(loop_)
				frame_ = ((int)frame_)%((int)maxframe_);
			else
				frame_ = maxframe_;
		}

		return false;
	}
