#pragma once

#include "axelynx/InstanceGroup.h"
#include "CInstance.h"
#include "CSurface.h"
#include <list>
#include <vector>
#include "gl/axgl.h"

class CInstanceGroup : public axelynx::InstanceGroup
{
	std::vector<axelynx::Instance*> instances_;
	const CSurface *surface_;

	mutable GLuint tbo_;
	mutable GLuint tex_;
public:
	CInstanceGroup();
	virtual const axelynx::Geometry* GetGeometry() const;
	virtual void SetGeometry(const axelynx::Geometry*);
	virtual axelynx::Instance* AddInstance();
	virtual bool RemoveInstance(axelynx::Instance *instance);

	virtual bool Draw() const;

	virtual int CountTriangles() const;

	virtual int CountVertices() const;

	virtual float GetRadius() const;

	virtual int GetBinarySize() const;
	virtual int WriteBinary(axelynx::File file) const;	
	virtual int RestoreFromBinary(axelynx::File file);

	virtual std::wstring GetClass() const
	{
		return L"instancegroup";
	}
};