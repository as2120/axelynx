
#include <iostream>

#include "CShape2D.h"
#include "CCanvas.h"
#include <string.h>
#include "CStatistics.h"

CShape2D::CShape2D(int cnttris, int cntverts)
	:vbo_(0)
	,ibo_(0)
	,vao_(0)
	,positions_(0)
	,colors_(0)
	,uv_(0)
	,uv_use_(true)
	,color_use_(true)
	,cntvertices_(cntverts)
	,cntindices_(cnttris*3)
	,drawmode_(GL_TRIANGLES)
	,vertexsize_(0)
	,vbuffer_(0)
{
	indexsize_ = 1;

	if(cntverts>255)
	{
		indexsize_ =2;

		if(cntverts>65535)
			indexsize_ =4;
	}

	positions_ = new vec2[cntverts];
	colors_ = new vec4[cntverts];
	uv_ = new vec2[cntverts];

	indices_ = new int[cnttris*3];
}

CShape2D::~CShape2D()
{
}

void CShape2D::Draw(const axelynx::Canvas *canvas) const
{
	if(!vao_)
		return;

	// для рендеринга исопльзуем VAO
	glBindVertexArray(vao_);
	// рендер треугольника из VBO привязанного к VAO
	//glDrawArrays(GL_LINES, 0, cntvertices_);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo_);
	//glBindBuffer(GL_ARRAY_BUFFER,vbo_);
	//glDepthMask(GL_TRUE);
	//glDisable(GL_DEPTH_TEST);
	canvas->ApplyTransform(CShader::Current());
	glDrawRangeElements(GL_TRIANGLES,0,cntindices_-1,cntindices_,GL_UNSIGNED_BYTE,0);

	CStatistics::dips++;
	CStatistics::tris_rendered++;
	CStatistics::vertices_rendered++;

	OPENGL_CHECK_FOR_ERRORS();
}

bool CShape2D::Build()
{
	if(!CShader::Current())
		return false;

	if(cntindices_>0) //есть тругольники
	{
		// создадим и используем Vertex Array Object (VAO)
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		// создадим и используем Vertex Buffer Object (VBO)
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		glGenBuffers(1, &ibo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

		int position_size = sizeof(GLfloat) * 2;
		int color_size = sizeof(GLubyte) *4;
		int uv_size = sizeof(GLushort) *2;

		std::cout<<"position_size"<<position_size<<std::endl;
		std::cout<<"color_size"<<color_size<<std::endl;
		std::cout<<"uv_size"<<uv_size<<std::endl;

		vertexsize_ = position_size;

		if(uv_use_)
		{
			vertexsize_ += uv_size;
		}

		if(color_use_)
		{
			vertexsize_ += color_size;
		}

		if(vbuffer_)
			delete[] vbuffer_;

		vbuffer_ = new GLubyte[cntvertices_ * vertexsize_];

		GLubyte *pointer = vbuffer_;

		for(int i=0;i<cntvertices_;++i)
		{
			memcpy(pointer,&positions_[i],sizeof(positions_[i]));
			pointer += sizeof(positions_[i]);
			std::cout<<"sizeof(positions_[i]):"<<sizeof(positions_[i])<<std::endl;

			if(uv_use_)
			{
				GLushort uv[2];

				uv[0] = static_cast<GLushort>(uv_[i].x * 65535.0f); //red
				uv[1] = static_cast<GLushort>(uv_[i].y * 65535.0f); //green

				memcpy(pointer,&uv,sizeof(uv));
				pointer += sizeof(uv);

				std::cout<<"sizeof(uv):"<<sizeof(uv)<<std::endl;
			}


			if(color_use_)
			{
				GLubyte color[4];
				color[0] = static_cast<GLubyte>(colors_[i].x * 255.0f); //red
				color[1] = static_cast<GLubyte>(colors_[i].y * 255.0f); //green
				color[2] = static_cast<GLubyte>(colors_[i].z * 255.0f); //blue
				color[3] = static_cast<GLubyte>(colors_[i].w * 255.0f); //alpha

				memcpy(pointer,color,sizeof(color));
				pointer += sizeof(color);
							std::cout<<"sizeof(color):"<<sizeof(color)<<std::endl;
			}

		}

		struct myvertex
		{
			float x,y;
			GLushort u,v;
			GLubyte r,g,b,a;
		};

		myvertex *a = reinterpret_cast<myvertex*>(vbuffer_);
		for(int i=0;i<4;++i)
		{
			myvertex &v = a[i];
			std::cout<<"x:"<<v.x<<"y:"<<v.y<<std::endl;
			std::cout<<"u:"<<v.u<<"v:"<<v.v<<std::endl;
			std::cout<<"r:"<<int(v.r)<<"r:"<<int(v.g)<<"g:"<<int(v.b)<<"b:"<<int(v.a)<<std::endl;
		}

		// заполним VBO данными треугольника
		glBufferData(GL_ARRAY_BUFFER, cntvertices_ * vertexsize_,
			vbuffer_, GL_STATIC_DRAW);

		if(indexsize_==1)
		{
			std::cout<<"tut"<<std::endl;
			GLubyte *indices = new GLubyte[cntindices_];
			for(int i=0;i<cntindices_;++i)
			{

				indices[i] = indices_[i];
				std::cout<<"indices["<<i<<"] = "<<indices[i]<<std::endl;
			}

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, cntindices_ * sizeof(indices[0]),
				indices, GL_STATIC_DRAW);
		}
		else
		{
			if(indexsize_==2)
			{
				GLushort *indices = new GLushort[cntindices_];
				for(int i=0;i<cntindices_;++i)
				{
					indices[i] = indices_[i];
				}

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, cntindices_ * sizeof(indices[0]),
					indices, GL_STATIC_DRAW);
			}
			else
			{
				GLuint *indices = new GLuint[cntindices_];
				for(int i=0;i<cntindices_;++i)
				{
					indices[i] = indices_[i];
				}

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, cntindices_ * sizeof(indices[0]),
					indices, GL_STATIC_DRAW);
			}
		}
		// получим позицию атрибута 'position' из шейдера



		int position_offset = 0;
		int current_offset = position_size;

		int uv_offset = 0;
		if(uv_use_)
		{
			uv_offset = current_offset;
			current_offset += uv_size;
		}

		int color_offset = 0;
		if(color_use_)
		{
			color_offset = current_offset;
			current_offset += color_size;
		}

		std::cout<<"vertexsize:"<<vertexsize_<<std::endl;
		std::cout<<"position_offset:"<<position_offset<<std::endl;
		std::cout<<"uv_offset:"<<uv_offset<<std::endl;
		std::cout<<"color_offset:"<<color_offset<<std::endl;

		int positionLocation = CShader::Current()->GetAttribLocation("position");
		if (positionLocation != -1)
		{
			// назначим на атрибут параметры доступа к VBO
			glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE,
				vertexsize_, reinterpret_cast<const GLvoid*>(position_offset));
			// разрешим использование атрибута
			glEnableVertexAttribArray(positionLocation);
		}

		if(uv_use_)
		{
			int colorLocation = CShader::Current()->GetAttribLocation("texcoord");
			if (colorLocation != -1)
			{
				glVertexAttribPointer(colorLocation, 2, GL_UNSIGNED_SHORT, GL_TRUE,
					vertexsize_, reinterpret_cast<const GLvoid*>(uv_offset));

				glEnableVertexAttribArray(colorLocation);
			}
		}

		if(color_use_)
		{
			int colorLocation = CShader::Current()->GetAttribLocation("color");
			if (colorLocation != -1)
			{
				glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE,
					vertexsize_, reinterpret_cast<const GLvoid*>(color_offset));

				glEnableVertexAttribArray(colorLocation);
			}
		}



	}

	return true;
}

int CShape2D::TrianglesCount() const
{
	return cntindices_ / 3;
}

int CShape2D::VerticesCount() const
{
	return cntvertices_ / 3;
}

vec2 CShape2D::GetPosition(int index) const
{
	return positions_[index];
}

vec4 CShape2D::GetColor(int index) const
{
	return colors_[index];
}

vec2 CShape2D::GetTexCoords(int index) const
{
	return uv_[index];
}

int CShape2D::GetTrisIndex(int tris, int corner) const
{
	return indices_[tris*3 + corner];
}

void CShape2D::SetPosition(int index, const vec2& position)
{
	positions_[index] = position;
}

void CShape2D::SetColor(int index, const vec4& color)
{
	colors_[index] = color;
}

void CShape2D::SetTexCoords(int index, const vec2& uv)
{
	uv_[index] = uv;
}

void CShape2D::SetTrisIndex(int tris, int corner, int vertexindex)
{
	indices_[tris*3 + corner] = vertexindex;
}

void CShape2D::SetTrisIndex(int tris, int corner0, int corner1, int corner2)
{
	indices_[tris*3 + 0] = corner0;
	indices_[tris*3 + 1] = corner1;
	indices_[tris*3 + 2] = corner2;
}
