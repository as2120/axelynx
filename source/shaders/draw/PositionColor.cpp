#include <axelynx\StandartShaders.h>

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Draw::PositionColor()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"uniform mat3x2 transform;\n"
						"in vec4 color;\n"
						"out vec4 fragmentcolor;\n"
						"vec2 VTransform(vec2 pos, mat3x2 m)\n"
						"{\n"
						"return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);\n"
						"}\n"
						"void main(void) {\n"
						"vec2 tpos = VTransform(position, transform);\n"
						"gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"in vec4 fragmentcolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = fragmentcolor;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->Compile();
	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Draw::Position()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"uniform mat3x2 transform;\n"
						"in vec4 color;\n"
						"out vec4 fragmentcolor;\n"
						"vec2 VTransform(vec2 pos, mat3x2 m)\n"
						"{\n"
						"return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);\n"
						"}\n"
						"void main(void) {\n"
						"vec2 tpos = VTransform(position, transform);\n"
						"gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);\n"
						"}\n";

	const char *fs =	"uniform vec4 canvascolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = canvascolor;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->Compile();
	return shader;
}
