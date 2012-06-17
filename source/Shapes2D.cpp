#include "CShape2D.h"

AXELYNX_API axelynx::Shape2D* axelynx::Shape2D::Rect(int width_, int height_,bool centred)
{
	float width = static_cast<float>(width_)*0.5f;
	float height = static_cast<float>(height_)*0.5f;
	CShape2D *shape = new CShape2D(2,4);

	if(centred)
	{
		shape->SetPosition(0,vec2(-width,-height));
		shape->SetPosition(1,vec2(width,-height));
		shape->SetPosition(2,vec2(width,height));
		shape->SetPosition(3,vec2(-width,height));
	}
	else
	{
		shape->SetPosition(0,vec2(0,0));
		shape->SetPosition(1,vec2(width*2,0));
		shape->SetPosition(2,vec2(width*2,height*2));
		shape->SetPosition(3,vec2(0,height*2));
	}

	shape->SetColor(0,vec4(0.5,0.5,0.5,0.5));
	shape->SetTexCoords(0,vec2(0,0));

	shape->SetColor(1,vec4(0.5,0.5,0.5,0.5));
	shape->SetTexCoords(1,vec2(1,0));

	shape->SetColor(2,vec4(0.5,0.5,0.5,0.5));
	shape->SetTexCoords(2,vec2(1,1));

	shape->SetColor(3,vec4(0.5,0.5,0.5,0.5));
	shape->SetTexCoords(3,vec2(0,1));

	shape->SetTrisIndex(0,0,1,2);
	shape->SetTrisIndex(1,0,2,3);

	shape->Build();
	return shape;
}