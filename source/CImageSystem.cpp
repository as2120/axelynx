
#include "CCanvas.h"
#include "CImageSystem.h"
#include <iostream>
#include "CStatistics.h"

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif

void CImageSystem::Draw(const axelynx::Canvas *canvas) const
{

	shader_->Bind();
	texture_->Bind();

	canvas->ApplyTransform(shader_);

	int sz = curimages_;
	//std::cout<<sz<<std::endl;
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	for(int patch = 0; patch<curimages_;)
	{
		int size = min(curimages_ - patch,16384);

		

		glBufferData(GL_ARRAY_BUFFER, size * 64, src_buffer_ + patch, GL_STATIC_DRAW);
		glDrawRangeElements(GL_TRIANGLES,0,size*6 -1,size*6,GL_UNSIGNED_SHORT,0);

		CStatistics::dips++;
		CStatistics::tris_rendered += size *4;
		CStatistics::vertices_rendered += size *2;

		patch += 16384;
	}
	curimages_ = 0;
}

CImage * CImageSystem::GetImage()
{
	CImage * img =  new CImage(this);

	return img;
}

CImage * CImageSystem::GetImage(const axelynx::vec2& corner0, const axelynx::vec2& corner1)
{
	CImage * img =   new CImage(this);

	return img;
}

CImageSystem::CImageSystem(axelynx::Texture *tex, int maximages)
{
	shader_ = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"in vec4 color;\n"
						"in vec2 texcoord;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"void main(void) {\n"
						"gl_Position   = projection * vec4(position.x, position.y, 0.0,1.0);\n"
						"fragmentuv = texcoord;\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"uniform sampler2D diffuse;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = fragmentcolor * texture(diffuse,fragmentuv);\n"
						"}\n";

	//"color = fragmentcolor * texture(diffuse,fragmentuv);\n"
	shader_->VertexSource(vs);
	shader_->FragmentSource(fs);

	shader_->Compile();
	shader_->Bind();

	maximages_ = maximages;
	texture_ = tex;

	src_buffer_ = new RectData[maximages_];
	curimages_ = 0;

	// создадим и используем Vertex Array Object (VAO)
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);


	GLushort *indices = new GLushort[maximages_*6];

	for(int i=0;i<maximages_;++i)
	{
		indices[i*6 + 0] = 0 + i*4;
		indices[i*6 + 1] = 1 + i*4;
		indices[i*6 + 2] = 2 + i*4;
		indices[i*6 + 3] = 0 + i*4;
		indices[i*6 + 4] = 2 + i*4;
		indices[i*6 + 5] = 3 + i*4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maximages_*12, indices, GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, maximages_ * 64, src_buffer_, GL_STATIC_DRAW);

	int position_loc = 0;
	int position_uv = 8;
	int position_color = 12;

	int location = shader_->GetAttribLocation("position");

	if (location != -1)
	{
		// назначим на атрибут параметры доступа к VBO
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
			16, reinterpret_cast<const GLvoid*>(position_loc));
		// разрешим использование атрибута
		glEnableVertexAttribArray(location);
	}

	location = shader_->GetAttribLocation("texcoord");
	if (location != -1)
	{
		glVertexAttribPointer(location, 2, GL_UNSIGNED_SHORT, GL_TRUE,
			16, reinterpret_cast<const GLvoid*>(position_uv));

		glEnableVertexAttribArray(location);
	}

	location = shader_->GetAttribLocation("color");
	if (location != -1)
	{
		glVertexAttribPointer(location, 4, GL_UNSIGNED_BYTE, GL_TRUE,
			16, reinterpret_cast<const GLvoid*>(position_color));

		glEnableVertexAttribArray(location);
	}

	delete[] indices;
	//std::cout<<"RectData size:"<<sizeof(RectData)<<std::endl;
}

CImageSystem::~CImageSystem()
{
}

void CImageSystem::AddGeom(const RectData &rd)
{
	if(curimages_<maximages_)
	{
		src_buffer_[curimages_] = rd;
		curimages_ ++;
	}
}
