#pragma once

#include "axelynx/Pivot.h"

class axelynx::Pivot::CPivot
{
	axelynx::vec3 position_;
	axelynx::vec3 scale_;

	axelynx::quat orientation_;

	mutable axelynx::mat4 local_;
	mutable axelynx::mat4 transform_;
	mutable axelynx::mat3 rotate_;
	mutable bool recalc_;

	void CalcTransform() const;

	typedef std::list<axelynx::Pivot*> Childs;
	Childs *childs_;
	axelynx::Pivot *parent_;

	mutable int scene_frame_;
public:
	CPivot();

	virtual void SetParent(axelynx::Pivot * parent);
	virtual axelynx::Pivot* GetParent() const;

	virtual int AddChild(axelynx::Pivot * child);
	virtual std::list<axelynx::Pivot*>* ChildsList();
	virtual std::list<const axelynx::Pivot*>* ChildsList() const;

	virtual void SetPosition(const axelynx::vec3& position);
	virtual void SetScale(const axelynx::vec3& scale);
	virtual void SetOrientation(const axelynx::quat& orient);
	virtual void SetRotate(const axelynx::vec3& angles);

	virtual const axelynx::vec3& GetPosition(bool global = true,bool immediate = true) const;
	virtual const axelynx::vec3& GetScale(bool global = true) const;
	virtual const axelynx::quat& GetOrientation(bool global = true) const;

	virtual const axelynx::mat3& GetOrientationMatrix() const;
	virtual const axelynx::mat4& GetTransformMatrix(bool immediate = true) const;
	virtual const axelynx::mat4& GetLocalMatrix() const;

	virtual void Translate(const axelynx::vec3& translate);

	virtual void Move(const axelynx::vec3& translate);
	virtual void Move(float value);

	virtual void MoveX(float value);
	virtual void MoveY(float value);
	virtual void MoveZ(float value);

	virtual void Turn(const axelynx::vec3& angles);
	virtual void Turn(const axelynx::quat& orient);

	virtual void PointOn(const axelynx::vec3& point);
	virtual bool isMustRecalcTM() const
	{
		return recalc_;
	}
	virtual ~CPivot();
};