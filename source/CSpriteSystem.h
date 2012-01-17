#pragma once

#include "axelynx/math/vec4.h"
#include "axelynx/SpriteSystem.h"
#include "gl/axgl.h"

#include <vector>

class CSpriteSystem : public axelynx::SpriteSystem
{
public:
	class CSprite : public axelynx::SpriteSystem::Sprite
	{
	public:
		float r,g,b,a;
		float size;
		virtual void SetColor(float r_, float g_, float b_, float a_)
		{
			r = r_;
			g = g_;
			b = b_;
			a = a_;
		}

		virtual void SetSize(float size_)
		{
			size = size_;
		}

		virtual ~CSprite(){};	
	};



private:
	typedef std::vector<CSprite*> Pivots;
	Pivots instances_;

	GLuint vao_;
	GLuint vbo_;
	GLuint uabo_;
	GLuint ibo_;

	struct Sprite
	{
		float x,y,z;
		float size;
		float r,g,b,a;
	};

	typedef std::vector<Sprite> Positions;
	mutable Positions positions_;

	axelynx::SpriteSystem::RecalcMode recalcmode_;

	mutable int old_size_;


	static void MTRecalc(void *ps);
public:
	struct params
	{
		const Pivots *instances;
		Positions *positions;
		int start;
		int end;
	};

	static params p;

	CSpriteSystem(int capacity_);

	virtual bool Draw() const;

	virtual int CountTriangles()const;
	virtual int CountVertices() const;

	virtual axelynx::SpriteSystem::Sprite* AddInstance();
	virtual bool RemoveInstance(axelynx::SpriteSystem::Sprite* pivot);

	virtual int SetDrawMode(DrawMode);
	virtual int SetRecalcMode(RecalcMode);

	virtual bool Recalc() const;
	virtual ~CSpriteSystem();

	virtual float GetRadius() const
	{
		return -1;
	}

	virtual int GetBinarySize() const;
	virtual int WriteBinary(axelynx::File file) const;	
	virtual int RestoreFromBinary(axelynx::File file);

	virtual std::wstring GetClass() const
	{
		return L"spritesystem";
	}
};