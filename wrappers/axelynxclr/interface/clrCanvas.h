#pragma once
#include <axelynx/axelynx.h>

public enum class BlendMode
{
	BM_NONE
	,BM_ALPHA
	,BM_MULTIPLY
	,BM_ADD
};

public ref class Canvas
{
	axelynx::Canvas *obj;
	public:
	Canvas(axelynx::Canvas *obj_):obj(obj_){}

	void Clear()
	{
		obj->Clear();
	}

	void ClearColor(float red, float green, float blue, float alpha)
	{
		obj->ClearColor(red,green,blue,alpha);
	}

	void SetFont(Font ^fnt)
	{
		obj->SetFont(fnt->obj);
	}

	void Text(String ^text, bool xcenter)
	{
		pin_ptr<const wchar_t> wch = PtrToStringChars(text);
		obj->Text(wch,xcenter);
	}

	/*virtual void SetColor(float red, float green, float blue)
	{
	}

	virtual vec4 GetColor() const
	{
	}

	virtual void SetAlpha(float alpha)
	{
	}





	virtual int GetHeight() const
	{
	}
	virtual int GetWidth() const
	{
	}

	virtual BlendMode CurrentBlendMode()
	{
		return static_cast<BlendMode>(obj->CurrentBlendMode()));
	}

	virtual void SetBlendMode(BlendMode blend)
	{
		obj->SetBlendMode(static_cast<axelynx::BlendMode>(blend));
	}*/

	/*


	virtual void Rect(float x, float y, float width, float height)=0;



	virtual int Draw(const axDrawable* some) const =0;



	virtual bool ApplyTransform(axelynx::Shader *shader) const=0;
	
	virtual ~Canvas(){};
	*/
};