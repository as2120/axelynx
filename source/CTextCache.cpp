

#include "CTextCache.h"

CTextCache::CTextCache(const axelynx::Font *fnt)
{
	shader_ = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"in vec2 texcoord;\n"
						"in vec4 color;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"void main(void) {\n"
						"gl_Position   = projection * vec4(position, 0.0,1.0);\n"
						"fragmentuv = vec2(texcoord.x,1.0-texcoord.y);\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"uniform sampler2D diffuse;\n"	
						"in vec2 fragmentuv;\n"
						"in vec4 fragmentcolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = vec4(fragmentcolor.rgb,fragmentcolor.a*texture2D(diffuse,fragmentuv).a);\n"
						"}\n";

	shader_->VertexSource(vs);
	shader_->FragmentSource(fs);

	shader_->BindAttribLocation("position",0);
	shader_->BindAttribLocation("texcoord",1);
	shader_->BindAttribLocation("color",2);

	shader_->Compile();

	shader_->Bind();
	shader_->SetUniform("diffuse",0);
	shader_->UnBind();

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	GLushort *indices = new GLushort[16384 * 6];

	for(unsigned int i=0;i<16384;++i)
	{
		indices[i*6 + 0] = 0 + i*4;
		indices[i*6 + 1] = 1 + i*4;
		indices[i*6 + 2] = 2 + i*4;
		indices[i*6 + 3] = 0 + i*4;
		indices[i*6 + 4] = 2 + i*4;
		indices[i*6 + 5] = 3 + i*4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16384 * 12, indices, GL_STATIC_DRAW);


	// назначим на атрибут параметры доступа к VBO
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		sizeof(StaticVertex), reinterpret_cast<const GLvoid*>(0));
	// разрешим использование атрибута
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE,
		sizeof(StaticVertex), reinterpret_cast<const GLvoid*>(8));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
		sizeof(StaticVertex), reinterpret_cast<const GLvoid*>(12));

	glEnableVertexAttribArray(2);
}

void CTextCache::Resize(int newsidth, int newheight)
{
}
		
void CTextCache::AddText(const char* ansi_string)
{
}

void CTextCache::AddText(const wchar_t* unicode_string)
{
}

void CTextCache::RemoveText(const char* ansi_string)
{
}

void CTextCache::RemoveText(const wchar_t* unicode_string)
{
}

void CTextCache::DrawText(const char* ansi_string)
{
}

void CTextCache::DrawText(const wchar_t* unicode_string)
{
}

void CTextCache::StartDraw(const axelynx::Canvas *c)
{
}

void CTextCache::EndDraw()
{
}


CTextCache::~CTextCache()
{
}