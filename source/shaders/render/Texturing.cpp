#include <axelynx/AxelynxSystemShaders.h>
#include "../../saSpec.h"
#include <axelynx/Logger.h>

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::Texturing()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in vec2 texcoord0;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentcolor = color;\n"
						"fragmentuv = texcoord0;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"			
						"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = texture2D(texture0,fragmentuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	//shader->Bind();


	//shader->UnBind();

	return shader;
}


AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::PointSprite()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in float pointsize;\n"
						"out vec4 fragmentcolor;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0) ;\n"
						"gl_PointSize = pointsize * 500.0f / gl_Position.z;\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"			
						"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = fragmentcolor * texture2D(texture0,vec2(gl_PointCoord.x,1.0-gl_PointCoord.y));\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_POINT_SIZE].name,VA_POINT_SIZE);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::FakeLines()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in float pointsize;\n"
						"out vec4 fragmentcolor;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0) ;\n"
						"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"in vec4 fragmentcolor;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = fragmentcolor;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_POINT_SIZE].name,VA_POINT_SIZE);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::QuadSprite()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in float pointsize;\n"
						"out float size;\n"
						"out vec4 vcolor;\n"
						"void main(void)\n"
						"{\n"
						"gl_Position  = modelviewproj * vec4( position, 1.0 );\n"
						"size         = pointsize * 0.5;\n"
						"vcolor         = color;\n"
						"}\n";
	//triangle_strip
	const char *gs =	"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 6) out;\n"
"uniform vec3 viewup;\n"
"uniform vec3 viewright;\n"
"in float size [];\n"
"in vec4 vcolor [];\n"
"out vec4 fragmentcolor;\n"
"out vec2 fragmentuv;\n"
"void main ()\n"
"{\n"
    "vec3	u = viewup * size [0];\n"
    "vec3	r = viewright * size [0];\n"
    "vec3	p = gl_in [0].gl_Position.xyz;\n"
    "float	w = gl_in [0].gl_Position.w;\n"
	"gl_Position = vec4 ( p - u - r, w );\n"
	"fragmentuv = vec2(0,0);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex ();\n"
    "gl_Position = vec4 ( p - u + r, w );\n"
	"fragmentuv = vec2(1,0);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex ();\n"
    "gl_Position = vec4 ( p + u + r, w );\n"
	"fragmentuv = vec2(1,1);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex   ();\n"
    "EndPrimitive ();\n"
    "gl_Position = vec4 ( p + u + r, w );\n"
	"fragmentuv = vec2(1,1);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex   ();\n"
    "gl_Position = vec4 ( p + u - r, w );\n"
	"fragmentuv = vec2(0,1);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex ();\n"
    "gl_Position = vec4 ( p - u - r, w );\n"
	"fragmentuv = vec2(0,0);\n"
	"fragmentcolor = vcolor[0];\n"
    "EmitVertex   ();\n"
   " EndPrimitive ();\n"
"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"			
						"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = fragmentcolor * texture2D(texture0,fragmentuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->GeometrySource(gs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_POINT_SIZE].name,VA_POINT_SIZE);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	//shader->Bind();


	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::VolumetricLines()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat4 modelview;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in vec3 endpos;\n"
						"in vec3 flow;\n"
						"in vec2 size;\n"
						"out vec4 fragmentcolor;\n"
						"out float fragmentblend;\n"
						"out vec2 fragmentuv0;\n"
						"out vec2 fragmentuv1;\n"
						"void main(void) {\n"
						"vec4 posstart = modelview * vec4(position,1.0);\n"
						"vec4 posend = modelview * vec4(endpos,1.0);\n"
						"vec3 middlepoint = normalize((posstart.xyz + posend.xyz)/2.0);\n"
						"vec3 lineoffset = posend.xyz - posstart.xyz;\n"
						"vec3 linedir = normalize(lineoffset);\n"
						"float sqlinelenght = dot(lineoffset, lineoffset);\n"
						"float texcoef = abs(dot(linedir, middlepoint));\n"
						"texcoef = max(((texcoef-1.0)*(sqlinelenght / size.y)) + 1.0 , 0.0);\n"
						"posstart = modelviewproj * vec4(position,1.0);\n"
						"posend = modelviewproj * vec4(endpos,1.0);\n"
						"vec2 startpos2d = posstart.xy / posstart.w;\n"
						"vec2 endpos2d = posend.xy / posend.w;\n"
						"vec2 linedir2d = normalize(startpos2d - endpos2d);\n"
						"posstart.xy = ((texcoef * flow.z) * linedir2d.xy) + posstart.xy;\n"
						"linedir2d = size.x * linedir2d;\n"
						"posstart.x = posstart.x + linedir2d.y;\n"
						"posstart.y= posstart.y - linedir2d.x;\n"
						"gl_Position   = posstart ;\n"
						"float  blend;\n"
						"vec2 tex;\n"
						"tex.y = min(15.0 / 16.0, texcoef);\n"
						"tex.x = modf(tex.y * 4.0, tex.y);\n"
						"blend = modf(tex.x * 4.0, tex.x);\n"
						"tex.xy = (tex.xy / 4.0) +(flow.xy);\n"
						"fragmentuv0 = tex;\n"
						"tex.y = min(texcoef + (1.0/16.0),15.0/16.0);\n"
						"tex.x = modf(tex.y * 4.0, tex.y);\n"
						"tex.x = floor(tex.x * 4.0);\n"
						"tex.xy = (tex.xy / 4.0) + (flow.xy);\n"
						"fragmentuv1 = tex;\n"
						"fragmentblend = blend;\n"
						"fragmentcolor = color;\n"
						"}\n";


	const char *fs =	"uniform sampler2D texture0;\n"
						"in vec4 fragmentcolor;\n"
						"in float fragmentblend;\n"
						"in vec2 fragmentuv0;\n"
						"in vec2 fragmentuv1;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"vec4 color0 = texture2D(texture0,fragmentuv0);\n"
						"vec4 color1 = texture2D(texture0,fragmentuv1);\n"
						"color = mix(color0,color1,fragmentblend) * fragmentcolor;\n"
						"}\n";

	/*const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat4 modelview;\n"
						"in vec3 position;\n"
						"in vec4 color;\n"
						"in vec3 endpos;\n"
						"in vec3 flow;\n"
						"in vec2 size;\n"
						"out vec4 fragmentcolor0;\n"
						"out float fragmentblend;\n"
						"out vec4 fragmentuv0;\n"
						"out vec4 fragmentuv1;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentcolor0 = color;\n"
						"}\n";


	const char *fs =	"uniform sampler2D texture0;\n"
						"in vec4 fragmentcolor0;\n"
						"in float fragmentblend;\n"
						"in vec4 fragmentuv0;\n"
						"in vec4 fragmentuv1;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"vec4 color0 = texture2D(texture0,fragmentuv0.xy);\n"
						"vec4 color1 = texture2D(texture0,fragmentuv1.xy);\n"
						"color = fragmentcolor0;\n"
						"}\n";*/
	//mix(color0,color1,fragmentblend) * fragmentcolor0 
	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation("flow",1);
	shader->BindAttribLocation("size",2);
	shader->BindAttribLocation("endpos",4);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::TexturingInstanced()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"#extension GL_EXT_gpu_shader4 : enable\n"
						"uniform mat4 modelviewproj;\n"
						"uniform samplerBuffer positions;\n"
						"in vec3 position;\n"
						//"in vec4 color;\n"
						"in vec2 texcoord0;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * (vec4(position,1.0) + texelFetchBuffer ( positions, gl_InstanceID ));\n"
						//"fragmentcolor = color;\n"
						"fragmentuv = texcoord0;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"			
						//"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = texture(texture0,fragmentuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	//shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	//shader->Bind();


	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::ZPassTexturingInstanced()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"#extension GL_EXT_gpu_shader4 : enable\n"
						"uniform mat4 modelviewproj;\n"
						"uniform samplerBuffer positions;\n"
						"in vec3 position;\n"
						//"in vec4 color;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * (vec4(position,1.0) + texelFetchBuffer ( positions, gl_InstanceID ));\n"
						//"fragmentcolor = color;\n"
						"}\n";

	const char *fs =	"void main(void) {\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	//shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	
	//shader->Bind();


	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::TransformOnly()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"}\n";

	const char *fs =	"out vec4 color;\n"
						"void main(void) {\n"
						"color = vec4(1.0,1.0,1.0,1.0);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);
	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->Compile();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::VolumetricRendering()
{
static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"in vec3 position;\n"
						"out vec4 fragmentcolor;\n"
						"out vec3 fragmentuv;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentuv = position;\n"
						"}\n";

	const char *fs =	"uniform sampler3D texture0;\n"			
						"in vec3 fragmentuv;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"color = texture(texture0,fragmentuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);

	shader->Compile();

	return shader;
}