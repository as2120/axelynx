
#include "CHGEFont.h"
#include "CFile.h"
#include <iostream>
#include <fstream>

#include "CShader.h"
#include "CCanvas.h"
#include "CTexture.h"
#include "CEngine.h"
#include "axelynx/utils/axstring.h"
#include "CStatistics.h"

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif
static unsigned char MakeByteFromHex(char first, char second)
{
	unsigned char result;

	if(first<'A')
		result = first - '0';
	else
		result = first - 'A' + 10;

	result *= 16;

	if(second<'A')
		result += (second - '0');
	else
		result += (second - 'A' + 10);

	return static_cast<char>(result);
}

CHGEFont::CHGEFont(const axelynx::Font::Desc &desc)
{
	//FILE *f = axReadFile(desc.filename.c_str());
	static_.reserve(16384);
	cur_canvas_ = 0;

    char fontfilename[255];
    utils::WideCharToMultiByte(desc.file.GetFilename().c_str(),fontfilename);

	std::ifstream file(fontfilename);

	std::string lex;

	std::string filename;

	int line = 0;

	int tex_width=0, tex_height = 0;

	glyfs_.resize(256);
	while(!file.eof())
	{
		unsigned char symbol = ' ';
		line++;

		char buff[1024];
		file.getline(buff,1024,10);

		if(line == 3)
		{
			filename = buff + 7;

			wchar_t  *filename_ = axelynx::utils::MultiByteToWideChar(filename.c_str());

			axelynx::Texture::Desc tdesc;
			tdesc.use_mipmaps = 0;
			//tdesc.internalbytes = 1;
			axelynx::PixMap* image = axelynx::Engine::Instance()->LoadPixMap(filename_);
			axelynx::PixMap* alphachannel = image->ExtractChannel(3);

			std::cout<<"achanel width:"<<alphachannel->Width()<<std::endl;
			std::cout<<"achanel height:"<<alphachannel->Height()<<std::endl;
			std::cout<<"achanel bpp:"<<alphachannel->Bpp()<<std::endl;

			tex_ = CTexture::FromPixMap(alphachannel,tdesc);
			tex_ = axelynx::Engine::Instance()->LoadTexture(filename_,tdesc);
			delete[] filename_;

			tex_width = tex_->GetWidth();
			tex_height = tex_->GetHeight();
		}

		if(line > 4)
		{
			int lb = 5;

			if(buff[5] == '"')
			{
				//32..127 char
				symbol = buff[6];
				lb = 9;
			}
			else
			{
				//other char
				symbol = MakeByteFromHex(buff[5],buff[6]);

				lb = 8;
			}

			int x_pos = 0;
			int y_pos = 0;
			int width = 0;
			int height = 0;
			int kern = 0;
			int x_shift = 0;

			sscanf(buff + lb,"%d,%d,%d,%d,%d,%d",&x_pos,&y_pos,&width,&height,&kern,&x_shift);


			Glyf g;

			g.u0 = static_cast<unsigned short>((x_pos-1) * 65535 / tex_width);
			g.v0 = static_cast<unsigned short>((y_pos-1) * 65535 / tex_height);
			g.u1 = static_cast<unsigned short>((x_pos+width-1) * 65535 / tex_width);
			g.v1 = static_cast<unsigned short>((y_pos+height-1) * 65535 / tex_height);

			g.width = static_cast<unsigned short>(width);
			g.height = static_cast<unsigned short>(height);

			g.kerning = kern;
			g.x_shift = x_shift + 1;
			glyfs_[symbol] = g;



		}
		//std::cout<<line<<":"<<buff<<" symbol:"<<symbol<<std::endl;
	}

	//std::cout<<"filename:"<<filename<<std::endl

	shader_ = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"uniform mat3x2 transform;\n"
						"in vec2 texcoord;\n"
						"out vec2 fragmentuv;\n"
						"vec2 VTransform(vec2 pos, mat3x2 m)\n"
						"{\n"
						"return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);\n"
						"}\n"
						"void main(void) {\n"
						"vec2 tpos = VTransform(position, transform);\n"
						"gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);\n"
						"fragmentuv = vec2(texcoord.x,1.0-texcoord.y);\n"
						"}\n";

	const char *fs =	"uniform sampler2D diffuse;\n"
						"uniform vec4 canvascolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = vec4(canvascolor.rgb,texture2D(diffuse,fragmentuv).a * canvascolor.a);\n"
						"}\n";

	shader_->VertexSource(vs);
	shader_->FragmentSource(fs);

	shader_->BindAttribLocation("position",0);
	shader_->BindAttribLocation("texcoord",1);

	shader_->Compile();

	shader_->Bind();
	shader_->SetUniform("diffuse",0);
	shader_->UnBind();


	shader_static_ = axelynx::Shader::Create();

	const char *svs =	"in vec2 position;\n"
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

	const char *sfs =	"uniform sampler2D diffuse;\n"
						"in vec2 fragmentuv;\n"
						"in vec4 fragmentcolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"float alpha = texture2D(diffuse,fragmentuv).a;\n"
						"color = vec4(fragmentcolor.rgb,fragmentcolor.a*alpha);\n"
						"}\n";

	shader_static_->VertexSource(svs);
	shader_static_->FragmentSource(sfs);

	shader_static_->BindAttribLocation("position",0);
	shader_static_->BindAttribLocation("texcoord",1);
	shader_static_->BindAttribLocation("color",2);

	shader_static_->Compile();

	shader_static_->Bind();
	shader_static_->SetUniform("diffuse",0);
	shader_static_->UnBind();

	file.close();

//region DYNAMIC

	// создадим и используем Vertex Array Object (VAO)
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	glGenBuffers(1, &ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	Vertex v[256];
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(Vertex) * 4, v, GL_STATIC_DRAW);
	GLubyte *indices = new GLubyte[384];

	for(int i=0;i<64;++i)
	{
		indices[i*6 + 0] = 0 + i*4;
		indices[i*6 + 1] = 1 + i*4;
		indices[i*6 + 2] = 2 + i*4;
		indices[i*6 + 3] = 0 + i*4;
		indices[i*6 + 4] = 2 + i*4;
		indices[i*6 + 5] = 3 + i*4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 384, indices, GL_STATIC_DRAW);

	// назначим на атрибут параметры доступа к VBO
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), reinterpret_cast<const GLvoid*>(0));
	// разрешим использование атрибута
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE,
		sizeof(Vertex), reinterpret_cast<const GLvoid*>(8));

	glEnableVertexAttribArray(1);



// создадим и используем Vertex Array Object (VAO)
	glGenVertexArrays(1, &vao_static_);
	glBindVertexArray(vao_static_);

	// создадим и используем Vertex Buffer Object (VBO)
	glGenBuffers(1, &vbo_static_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_static_);

	glGenBuffers(1, &ibo_static_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_static_);
	{
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
	}

	std::cout<<"SizeOf:"<<sizeof(StaticVertex)<<std::endl;

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
	//delete[] indices;
//endregion
}

bool CHGEFont::Bind() const
{
	return true;
}

bool CHGEFont::UnBind() const
{
	return true;
}

void CHGEFont::Draw(const axelynx::Canvas *canvas,const char *ansi_string, bool center) const
{
	//prepare buffer
	size_t l = strlen(ansi_string);
	if(l==0)
		return;

	//render text
	if(!cur_canvas_)
	{
		shader_->Bind();
		tex_->Bind();

		canvas->ApplyTransform(shader_);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

		Vertex v[256];

		int start_x = 0;
		int current_x = start_x;

		for(int i=0;i<l;++i)
		{
			unsigned int c = (unsigned char)ansi_string[i];

			const Glyf &g = glyfs_[c];

			int index = i * 4;
			current_x += g.kerning;

			v[index].x = static_cast<float>(current_x);
			v[index].y = 0;
			v[index].u = g.u0;
			v[index].v = g.v0;

			index++;
			v[index].x = static_cast<float>(current_x+g.width);
			v[index].y = 0;
			v[index].u = g.u1;
			v[index].v = g.v0;

			index++;
			v[index].x = static_cast<float>(current_x+g.width);
			v[index].y = g.height;
			v[index].u = g.u1;
			v[index].v = g.v1;

			index++;
			v[index].x = static_cast<float>(current_x);
			v[index].y = g.height;
			v[index].u = g.u0;
			v[index].v = g.v1;

			current_x += g.width + g.x_shift;
		}

		glBufferSubData(GL_ARRAY_BUFFER, 0, l * sizeof(Vertex) * 4, v);
		glDrawRangeElements(GL_TRIANGLES,0,l*6 -1,l*6,GL_UNSIGNED_BYTE,0);

		CStatistics::dips++;
		CStatistics::tris_rendered += l *4;
		CStatistics::vertices_rendered += l *2;
	}
	else
	{
		//Static draw
		int start_x = 0;
		int current_x = start_x;
		StaticVertex v;

		axelynx::vec4 canvas_color = cur_canvas_->GetColor();

		unsigned char red = static_cast<unsigned char>(canvas_color.x * 255.0f);
		unsigned char green = static_cast<unsigned char>(canvas_color.y * 255.0f);
		unsigned char blue = static_cast<unsigned char>(canvas_color.z * 255.0f);
		unsigned char alpha = static_cast<unsigned char>(canvas_color.w * 255.0f);

		axelynx::mat3x2 transform = cur_canvas_->GetTransform();

		int i = 0;
		unsigned int c = (unsigned char)ansi_string[i];

		const Glyf &g = glyfs_[c];

		current_x += g.kerning;

		v.position = transform * vec2(static_cast<float>(current_x),0.0f);
		v.u = g.u0;
		v.v = g.v0;
		v.r = red;
		v.g = green;
		v.b = blue;
		v.a = alpha;
		static_.push_back(v);

		v.position = transform * vec2(static_cast<float>(current_x+g.width),0.0f);
		v.u = g.u1;
		v.v = g.v0;
		v.r = red;
		v.g = green;
		v.b = blue;
		v.a = alpha;
		static_.push_back(v);

		v.position = transform * vec2(static_cast<float>(current_x+g.width),static_cast<float>(g.height));
		v.u = g.u1;
		v.v = g.v1;
		v.r = red;
		v.g = green;
		v.b = blue;
		v.a = alpha;
		static_.push_back(v);

		v.position = transform * vec2(static_cast<float>(current_x),static_cast<float>(g.height));
		v.u = g.u0;
		v.v = g.v1;
		v.r = red;
		v.g = green;
		v.b = blue;
		v.a = alpha;
		static_.push_back(v);

		current_x += g.width + g.x_shift;


		for(int i=1;i<l;++i)
		{
			unsigned int c = (unsigned char)ansi_string[i];

			const Glyf &g = glyfs_[c];

			current_x += g.kerning;
			size_t sz = static_.size();
			v.position = static_[sz-3].position;
			v.u = g.u0;
			v.v = g.v0;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = transform * vec2(static_cast<float>(current_x+g.width),0.0f);
			v.u = g.u1;
			v.v = g.v0;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = transform * vec2(static_cast<float>(current_x+g.width),static_cast<float>(g.height));
			v.u = g.u1;
			v.v = g.v1;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = static_[sz-2].position;
			v.u = g.u0;
			v.v = g.v1;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			current_x += g.width + g.x_shift;
		}
	}
}

void CHGEFont::Draw(const axelynx::Canvas *c,const wchar_t *wide_string, bool center) const
{
	static char buff[1024];
	axelynx::utils::WideCharToMultiByte(wide_string, buff);

	Draw(c,buff,center);
}

int CHGEFont::GetHeight() const
{
	return 0;
}

int CHGEFont::GetWidth(const char *ansi_string) const
{
	int result =0;
	size_t l = strlen(ansi_string);
	for(int i=0;i<l;++i)
	{
		const Glyf &g = glyfs_[ansi_string[i]];

		result += g.width + g.x_shift + g.kerning;
	}

	return 0;
}

int CHGEFont::GetWidth(const wchar_t *wide_string) const
{
	static char buff[1024];
	axelynx::utils::WideCharToMultiByte(wide_string, buff);

	return GetWidth(buff);
}

CHGEFont::~CHGEFont()
{
}

void CHGEFont::StartDraw(const axelynx::Canvas *c) const
{

	cur_canvas_ = c;
	static_.clear();
}

void CHGEFont::EndDraw() const
{
	if(cur_canvas_)
	{
		shader_static_->Bind();
		tex_->Bind();

		cur_canvas_->ApplyTransform(shader_static_);

		glBindVertexArray(vao_static_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_static_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_static_);

		size_t curimages = static_.size() / 4;
		for(int patch = 0; patch<curimages;)
		{
			int size = min(curimages - patch,16384);



			glBufferData(GL_ARRAY_BUFFER, size * 64, static_.data() + patch, GL_STATIC_DRAW);
			glDrawRangeElements(GL_TRIANGLES,0,size*6 -1,size*6,GL_UNSIGNED_SHORT,0);

			CStatistics::dips++;
			CStatistics::tris_rendered += size *4;
			CStatistics::vertices_rendered += size *2;

			patch += 16384;
		}
	}



	cur_canvas_ = 0;
}

