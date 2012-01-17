#pragma once

#include "axelynx/ParticleSystem.h"

class CParticleSystem : public axelynx::ParticleSystem
{
public:
	CParticleSystem();

	virtual bool Draw(float frame) const;
	virtual bool Update(float tween = 1.0f) const;
	virtual int CountTriangles()const;
	virtual int CountVertices() const;

	virtual int SetDrawMode(DrawMode);
	virtual int SetUpdateMode(DrawMode);

	virtual ~CParticleSystem();

	virtual int GetBinarySize() const;
	virtual int WriteBinary(axelynx::File file) const;	
	virtual int RestoreFromBinary(axelynx::File file);

	virtual std::wstring GetClass() const
	{
		return L"particlesystem";
	}
};