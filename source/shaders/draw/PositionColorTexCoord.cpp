#include <axelynx/AxelynxSystemShaders.h>

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Draw::PositionColorUV()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"uniform mat3x2 transform;\n"
						"in vec4 color;\n"
						"in vec2 texcoord;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"vec2 VTransform(vec2 pos, mat3x2 m)\n"
						"{\n"
						"return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);\n"
						"}\n"
						"void main(void) {\n"
						"vec2 tpos = VTransform(position, transform);\n"
						"gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);\n"
						"fragmentcolor = color;\n"
						"fragmentuv = texcoord;\n"
						"}\n";

	const char *fs =	"uniform sampler2D diffuse;\n"			
						"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = vec4(color.rgb,texture2D(diffuse,fragmentuv).r * color.a);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->Compile();
	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Draw::PositionUV()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

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
						"fragmentuv = texcoord;\n"
						"}\n";

	const char *fs =	"uniform sampler2D diffuse;\n"	
						"uniform vec4 ucolor;\n"	
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = texture2D(diffuse,fragmentuv)*ucolor;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->Compile();
	return shader;
}