
#include "CFreeTypeFont.h"

#include "CFile.h"
#include <iostream>
#include <fstream>

#include "CShader.h"
#include "CCanvas.h"
#include "CTexture.h"
#include "CEngine.h"
#include "axelynx/utils/axstring.h"
#include "CStatistics.h"

#ifdef WIN32
#include <ShlObj.h>
#endif

#ifndef min
#define min(a,b) ((a)<(b))?(a):(b)
#endif

int CFreeTypeFont::count_fonts = 0;
FT_Library CFreeTypeFont::library_;

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

CFreeTypeFont::CFreeTypeFont(const axelynx::Font::Desc &desc)
{
	//std::cout<<"freetype font size:"<<desc.size<<std::endl;
	//FILE *f = axReadFile(desc.filename.c_str());
	static_.reserve(16384);
	cur_canvas_ = 0;

	file_buff = 0;

	
	glyfs_ = new Glyf[65536];

	FT_Init_FreeType( &library_ );

	std::wstring filename = desc.file.GetFilename();

	if(filename != L"") //disk file
	{
#ifdef WIN32
		if(!axFileType(filename.c_str())) //нет файла
		{
			wchar_t wbuff[260];
			SHGetFolderPathW(0,0x0014,0,0,wbuff);
			filename = std::wstring(wbuff) + L"/"+filename;
		}
#endif
		char fontfilename[255];
		axelynx::utils::WideCharToMultiByte(filename.c_str(),fontfilename);
		FT_New_Face( library_, fontfilename, 0,&face_ );
	}
	else //memory file
	{
		axelynx::File file = desc.file;
		file.Open();
		FT_Long size = static_cast<FT_Long>(file.Size());
		file_buff = new unsigned char[size];
		file.Read(file_buff,size);

		file.Close();
		FT_New_Memory_Face(library_,file_buff,size,0,&face_);
	}



	


	FT_Set_Char_Size( face_, 0, desc.size * 64, 96, 96);
	
	FT_Set_Pixel_Sizes(
            face_,   /* handle to face object */
            0,      /* pixel_width           */
            desc.size );   /* pixel_height          */

	FT_Select_Charmap(face_,FT_ENCODING_UNICODE);

	axelynx::Texture::Desc tdesc;
	tdesc.Size(1024,1024).UseMipmaps(false);
	tdesc.cpp = 1;

	unsigned char *data = new unsigned char[1024 * 1024 *4];
	for(int i=0;i<1024 * 1024 *4; ++i)
	{
		data[i] = (rand() % 255);
	}

	CTexture *ctex = new CTexture(tdesc);
	ctex->Build(0,GL_RED,GL_R8);
	delete[] data;

	tex_ = ctex;

	cx_ = 0;
	cy_ = 0;

	font_height_ = desc.size;


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
						"fragmentuv = vec2(texcoord.x,texcoord.y);\n"
						"}\n";

	const char *fs =	"uniform sampler2D diffuse;\n"
						"uniform vec4 canvascolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = vec4(canvascolor.rgb,canvascolor.a * texture(diffuse,fragmentuv).r);\n"
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
						"fragmentuv = vec2(texcoord.x,texcoord.y);\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *sfs =	"uniform sampler2D diffuse;\n"
						"in vec2 fragmentuv;\n"
						"in vec4 fragmentcolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"float alpha = texture2D(diffuse,fragmentuv).r;\n"
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

//	std::cout<<"SizeOf:"<<sizeof(StaticVertex)<<std::endl;

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

	cur_canvas_ = 0;
}

CFreeTypeFont::Glyf CFreeTypeFont::RenderGyph(wchar_t symbol) const
{
		Glyf glyf;
		
		FT_GlyphSlot  slot = face_->glyph;  /* a small shortcut */

		FT_UInt  glyph_index;


		/* retrieve glyph index from character code */
		glyph_index = FT_Get_Char_Index( face_, symbol );

		/* load glyph image into the slot (erase previous one) */
		FT_Load_Glyph( face_, glyph_index, FT_LOAD_RENDER);


		/* convert to an anti-aliased bitmap */
		//FT_Render_Glyph( face_->glyph, FT_RENDER_MODE_NORMAL );
		
		glyf.height = font_height_;
		glyf.width = slot->bitmap.width;
		glyf.kerning = 0;
		glyf.x_shift = slot->bitmap_left;
		FT_Pos val = (slot->metrics.height - slot->metrics.horiBearingY);
		glyf.y_shift = static_cast<char>(static_cast<float>(val) / (26.6f * 2.0f));

		if(cx_ + slot->bitmap.width > 1024)
		{
			cy_ += font_height_;
			cx_ = 0;
		}

		glyf.u0 = static_cast<unsigned short>((static_cast<float>(cx_) / 1024.0f) * 65535);
		glyf.v0 = static_cast<unsigned short>((static_cast<float>(cy_) / 1024.0f) * 65535);
		glyf.u1 = static_cast<unsigned short>((static_cast<float>(cx_+slot->bitmap.width) / 1024.0f) * 65535);
		glyf.v1 = static_cast<unsigned short>((static_cast<float>(cy_+glyf.height) / 1024.0f) * 65535);

		//std::cout<<(unsigned char)symbol<<std::endl;
		//std::cout<<"width"<<glyf.width<<std::endl;
		//std::cout<<"height"<<glyf.height<<std::endl;

		//std::cout<<"bwidth"<<slot->bitmap.width<<std::endl;
		//std::cout<<"bheight"<<slot->bitmap.rows<<std::endl;


		if(slot->bitmap.buffer)
		{
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			glTexSubImage2D(GL_TEXTURE_2D,0,cx_,cy_ + glyf.height - slot->bitmap.rows,slot->bitmap.width,slot->bitmap.rows,GL_RED,GL_UNSIGNED_BYTE,slot->bitmap.buffer);
		}

		cx_ += slot->bitmap.width;
		glyf.generated  =true;

		glyfs_[symbol] = glyf;

	return glyf;
}

bool CFreeTypeFont::Bind() const
{
	return true;
}

bool CFreeTypeFont::UnBind() const
{
	return true;
}

void CFreeTypeFont::Draw(const axelynx::Canvas *canvas,const char *ansi_string, bool center) const
{
	
}

void CFreeTypeFont::Draw(const axelynx::Canvas *canvas,const wchar_t *wide_string, bool center) const
{
	static char buff[1024];
	axelynx::utils::WideCharToMultiByte(wide_string, buff);

	tex_->Bind();
	size_t l = wcslen(wide_string);

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
			wchar_t c = (wchar_t)wide_string[i];

			if(!glyfs_[wide_string[i]].generated)
				RenderGyph(wide_string[i]);

			const Glyf &g = glyfs_[wide_string[i]];

			int index = i * 4;
			current_x += g.kerning;

			v[index].x = static_cast<float>(current_x+g.x_shift);
			v[index].y = g.y_shift;
			v[index].u = g.u0;
			v[index].v = g.v0;

			index++;
			v[index].x = static_cast<float>(current_x+g.width+g.x_shift);
			v[index].y = g.y_shift;
			v[index].u = g.u1;
			v[index].v = g.v0;

			index++;
			v[index].x = static_cast<float>(current_x+g.width+g.x_shift);
			v[index].y = g.y_shift + static_cast<float>(g.height);
			v[index].u = g.u1;
			v[index].v = g.v1;

			index++;
			v[index].x = static_cast<float>(current_x+g.x_shift);
			v[index].y = g.y_shift + static_cast<float>(g.height);
			v[index].u = g.u0;
			v[index].v = g.v1;

			current_x += g.width + g.x_shift + 2;
			if(c == L' ')
				current_x += 4;
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
		wchar_t c = wide_string[i];

		if(!glyfs_[wide_string[i]].generated)
			RenderGyph(wide_string[i]);

		for(int i=0;i<l;++i)
		{
			if(!glyfs_[wide_string[i]].generated)
				RenderGyph(wide_string[i]);

			const Glyf &g = glyfs_[wide_string[i]];

			current_x += g.kerning;

			v.position = transform * vec2(static_cast<float>(current_x + g.x_shift),static_cast<float>(g.y_shift));
			v.u = g.u0;
			v.v = g.v0;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = transform * vec2(static_cast<float>(current_x+g.width + g.x_shift),static_cast<float>(g.height+g.y_shift));
			v.u = g.u1;
			v.v = g.v0;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = transform * vec2(static_cast<float>(current_x+g.width + g.x_shift),static_cast<float>(g.height+g.y_shift));
			v.u = g.u1;
			v.v = g.v1;
			v.r = red;
			v.g = green;
			v.b = blue;
			v.a = alpha;
			static_.push_back(v);

			v.position = transform * vec2(static_cast<float>(current_x + g.x_shift),static_cast<float>(g.height+g.y_shift));
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

	tex_->UnBind();
	//Draw(c,buff,center);
}

int CFreeTypeFont::GetHeight() const
{
	return 0;
}

int CFreeTypeFont::GetWidth(const char *ansi_string) const
{
	int result =0;
	size_t l = strlen(ansi_string);
	for(size_t i=0;i<l;++i)
	{
		const Glyf &g = glyfs_[ansi_string[i]];

		result += g.width + g.x_shift + g.kerning;
	}

	return 0;
}

int CFreeTypeFont::GetWidth(const wchar_t *wide_string) const
{
	static char buff[1024];
	axelynx::utils::WideCharToMultiByte(wide_string, buff);

	return GetWidth(buff);
}

CFreeTypeFont::~CFreeTypeFont()
{

  FT_Done_Face(face_);

  if(file_buff)
	  delete[] file_buff;

  FT_Done_FreeType(library_);

}

void CFreeTypeFont::StartDraw(const axelynx::Canvas *c) const
{

	cur_canvas_ = c;
	static_.clear();
}

void CFreeTypeFont::EndDraw() const
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

















































































