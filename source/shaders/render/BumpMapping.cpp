#include <axelynx/AxelynxSystemShaders.h>
#include "../../saSpec.h"
#include <axelynx/Logger.h>

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::BumpMappingAdvanced()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 model;\n"
						"uniform mat4 view;\n"
						"uniform mat4 proj;\n"
						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec3 tangent;\n"
						"in vec4 color;\n"
						"in vec2 texcoord0;\n"
						"uniform vec3 lightpos0;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"out vec3 fragmentn;\n"
						"out vec3 fragmentt;\n"
						"out vec3 viewdir;\n"
						"out vec3 lightdir;\n"
						"out float fragmentz;\n"
						"void main(void) {\n"
						"vec4 npos = model * vec4(position,1.0);"
						"viewdir = normalize(npos.xyz - vec3(view[3][0],view[3][1],view[3][2]));\n"
						"lightdir = normalize(lightpos0);\n"
						"gl_Position   = proj * view * npos;\n"
						"fragmentz = gl_Position.z;"
						"fragmentcolor = color;\n"
						"mat4 turn = model;\n"
						"turn[3][0]=0;turn[3][1]=0;turn[3][2]=0;\n"
						"fragmentn = (turn * vec4(normal,1)).rgb;\n"
						"fragmentt = (turn * vec4(tangent,1)).rgb;\n"
						"fragmentuv = texcoord0;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0; //diffuse\n"
						"uniform sampler2D texture1; //normals\n"
						"in vec4 fragmentcolor;\n"
						"in vec2 fragmentuv;\n"
						"in vec3 fragmentn;\n"
						"in vec3 fragmentt;\n"
						"in vec3 viewdir;\n"
						"in vec3 lightdir;\n"
						"in float fragmentz;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"vec3 normal = normalize(fragmentn + (texture(texture1, fragmentuv).rgb - vec3(0.5,0.5,0.5)) * 2.0);\n"
						"float diff = max(dot(normal,lightdir),0);"
						"diff = pow(diff,2.0)*1.2;\n"
						"float RdotVpow = max(pow(dot(reflect(lightdir, normal), viewdir), 4.0), 0.0);\n"
						"diff += RdotVpow;\n"
						"float fade = min(1.0-(fragmentz/4000.0f),1.0);\n"
						"diff *= fade;\n"
						"color = texture2D(texture0,fragmentuv) * diff;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	shader->BindAttribLocation(sysattribs[VA_COLOR].name,VA_COLOR);

	shader->Compile();

	//shader->Bind();
	

	//shader->UnBind();

	return shader;
}


AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::BumpLightMapping()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat3 normalmatrix;\n"
						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec3 tangent;\n"
						"in vec2 texcoord0;\n"
						"in vec2 texcoord1;\n"
						"out vec2 fragmentuv;\n"
						"out vec2 lightuv;\n"
						"out vec3 fragmentn;\n"
						"out vec3 fragmentt;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentn = normalmatrix * normal;\n"
						"fragmentt = normalmatrix * tangent;\n"
						"fragmentuv = texcoord0;\n"
						"lightuv = texcoord1;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0; //diffuse\n"
						"uniform sampler2D texture1; //normals\n"
						"uniform sampler2D texture2; //lightmap\n"
						"in vec2 fragmentuv;\n"
						"in vec2 lightuv;\n"
						"in vec3 fragmentn;\n"
						"in vec3 fragmentt;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"vec3 normal = normalize(fragmentn + (texture(texture1, fragmentuv).rgb - vec3(0.5,0.5,0.5)) * 2.0);\n"
						"float diff = max(dot(normal,normalize(vec3(-250,20,100)))*2.0,0);"
						"color = diff * texture2D(texture0,fragmentuv) * texture2D(texture2,lightuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD1].name,VA_TEXCOORD1);

	shader->Compile();

	//shader->Bind();
	

	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::OnlyNormals()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat3 normalmatrix;\n"
						"uniform vec3 lightpos;\n"
						"uniform mat4 model;\n"
						"in vec3 position;\n"
						"in vec3 normal;\n"
						"out vec3 fragmentn;\n"
						"out vec3 lightvec;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentn = normalmatrix * normal;\n"
						"lightvec = lightpos - (model * vec4(position,1.0)).xyz;\n"
						"}\n";

	const char *fs =	"in vec3 fragmentn;\n"
						"in vec3 lightvec;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"float diff = max(dot(normalize(fragmentn),normalize(lightvec)),0.01);\n"
						"color = vec4(diff,diff,diff,1.0);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);

	shader->Compile();
	shader->SetUniform(axelynx::Shader::SU_LIGHTPOS,axelynx::vec3(-100,200,-400));
	//shader->Bind();
	

	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::TexturingLighting()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat3 normalmatrix;\n"
						"uniform vec3 lightpos;\n"
						"uniform mat4 model;\n"
						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec2 texcoord0;\n"
						"out vec3 fragmentn;\n"
						"out vec2 fragmentuv;\n"
						"out vec3 lightvec;\n"
						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentn = normalmatrix * normal;\n"
						"lightvec = lightpos - (model * vec4(position,1.0)).xyz;\n"
						"fragmentuv = texcoord0;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"
						"in vec3 fragmentn;\n"
						"in vec2 fragmentuv;\n"
						"in vec3 lightvec;\n"
						"out vec4 color;\n"
						"void main(void) {\n"
						"float diff = max(dot(normalize(fragmentn),normalize(lightvec)),0.01);\n"
						"color = diff * texture2D(texture0,fragmentuv);\n"
						"}\n";

	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);

	
	shader->Compile();
	shader->SetUniform(axelynx::Shader::SU_LIGHTPOS,axelynx::vec3(-100,200,-400));
	//shader->Bind();
	

	//shader->UnBind();

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::BumpMapping()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat4 model;\n"
						"uniform mat4 modelview;\n"
						"uniform mat3 normalmatrix;\n"
						"uniform vec3 lightpos;\n"

						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec3 tangent;\n"
						"in vec2 texcoord0;\n"
						
						"out vec3 lightVec;\n"
						"out vec3 eyeVec;\n"
						"out vec2 fragmentuv;\n"

						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentuv =texcoord0;\n"

						"vec3 n = normalize(normalmatrix * normal);\n"
						"vec3 t = normalize(normalmatrix * tangent);\n"
						"vec3 b = cross(n, t);\n"

						"vec3 vVertex = vec3(model * vec4(position,1));\n"
						"vec3 tmpVec = lightpos - vVertex;\n"

						"lightVec.x = dot(tmpVec, t);\n"
						"lightVec.y = dot(tmpVec, b);\n"
						"lightVec.z = dot(tmpVec, n);\n"
						
						"tmpVec = -vec3(modelview * vec4(position,1));\n"
						"eyeVec.x = dot(tmpVec, t);\n"
						"eyeVec.y = dot(tmpVec, b);\n"
						"eyeVec.z = dot(tmpVec, n);\n"

						"}\n";

	const char *fs =	"uniform sampler2D texture0; //diffuse\n"
						"uniform sampler2D texture1; //normals\n"
						"uniform float lightRadius;\n"
						"uniform float specular;\n"
						"in vec3 lightVec;\n"
						"in vec3 eyeVec;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"

						"void main(void) {\n"
						"float distSqr = dot(lightVec, lightVec);\n"
						"float att = clamp(1.0 - lightRadius * sqrt(distSqr), 0.0, 1.0);\n"
						"vec3 lVec = lightVec * inversesqrt(distSqr);\n"

						"vec3 vVec = normalize(eyeVec);\n"
						
						"vec4 base = texture(texture0, fragmentuv);\n"
						
						"vec3 bump = normalize( texture2D(texture1, fragmentuv).xyz * 2.0 - 1.0);\n"
										
						"float diffuse = max( dot(lVec, bump), 0.0 );\n"
												
						"float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 8.0 );\n"
							
						"color = ((diffuse + specular)*base) * att;\n"
						"}\n";


	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);

	shader->SetUniform("lightRadius",1.0f / 100.0f);
	shader->SetUniform("specular",0.1f);

	shader->Compile();

	shader->SetUniform(axelynx::Shader::SU_LIGHTPOS,axelynx::vec3(-100,200,-400));

	return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::MorfedMeshBumpMapping()
{

static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat4 model;\n"
						"uniform mat4 modelview;\n"
						"uniform mat3 normalmatrix;\n"
						"uniform vec3 lightpos;\n"

						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec3 tangent;\n"
						"in vec2 texcoord0;\n"
						
						"out vec3 lightVec;\n"
						"out vec3 eyeVec;\n"
						"out vec2 fragmentuv;\n"

						"void main(void) {\n"
						"gl_Position   = modelviewproj * vec4(position,1.0);\n"
						"fragmentuv =texcoord0;\n"

						"vec3 n = normalize(normalmatrix * normal);\n"
						"vec3 t = normalize(normalmatrix * tangent);\n"
						"vec3 b = cross(n, t);\n"

						"vec3 vVertex = vec3(model * vec4(position,1));\n"
						"vec3 tmpVec = lightpos - vVertex;\n"

						"lightVec.x = dot(tmpVec, t);\n"
						"lightVec.y = dot(tmpVec, b);\n"
						"lightVec.z = dot(tmpVec, n);\n"
						
						"tmpVec = -vec3(modelview * vec4(position,1));\n"
						"eyeVec.x = dot(tmpVec, t);\n"
						"eyeVec.y = dot(tmpVec, b);\n"
						"eyeVec.z = dot(tmpVec, n);\n"

						"}\n";

	const char *fs =	"uniform sampler2D texture0; //diffuse\n"
						"uniform sampler2D texture1; //normals\n"
						"uniform float lightRadius;\n"
						"uniform float specular;\n"
						"in vec3 lightVec;\n"
						"in vec3 eyeVec;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"

						"void main(void) {\n"
						"float distSqr = dot(lightVec, lightVec);\n"
						"float att = clamp(1.0 - lightRadius * sqrt(distSqr), 0.0, 1.0);\n"
						"vec3 lVec = lightVec * inversesqrt(distSqr);\n"

						"vec3 vVec = normalize(eyeVec);\n"
						
						"vec4 base = texture(texture0, fragmentuv);\n"
						
						"vec3 bump = normalize( texture2D(texture1, fragmentuv).xyz * 2.0 - 1.0);\n"
										
						"float diffuse = max( dot(lVec, bump), 0.0 );\n"
												
						"float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 8.0 );\n"
							
						"color = ((diffuse + specular)*base) * att;\n"
						"}\n";


	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);

	shader->SetUniform("lightRadius",1.0f / 100.0f);
	shader->SetUniform("specular",0.1f);

	shader->Compile();

	shader->SetUniform(axelynx::Shader::SU_LIGHTPOS,axelynx::vec3(-100,200,-400));

	return shader;

	//static axelynx::Shader * shader=0;
	//if(shader)
	//	return shader;

	//shader = axelynx::Shader::Create();

	//const char *vs =	"uniform mat4 modelviewproj;\n"
	//					"uniform mat4 model;\n"
	//					"uniform mat3 normalmatrix;\n"
	//					"uniform float scalar;\n"
	//					"uniform vec3 lightpos;\n"
	//					"in vec3 position;\n"
	//					"in vec3 normal;\n"
	//					"in vec3 tangent;\n"
	//					"in vec2 texcoord0;\n"
	//					"in vec3 nextposition;\n"
	//					"in vec3 nextnormal;\n"
	//					"in vec3 nexttangent;\n"
	//					"in vec2 nexttexcoord0;\n"
	//					"out vec2 fragmentuv;\n"
	//					"out vec3 fragmentnormal;\n"
	//					"out vec3 lightvec;\n"
	//					"void main(void) {\n"
	//					"vec3 calcedpos = mix(position,nextposition,scalar);\n"
	//					"gl_Position   = modelviewproj * vec4(calcedpos,1.0);\n"
	//					"fragmentnormal = normalmatrix * mix(normal,nextnormal,scalar);\n"
	//					"fragmentuv = texcoord0;\n"
	//					"lightvec = (lightpos - (model * vec4(calcedpos,1.0)).xyz);\n"
	//					"}\n";

	//const char *fs =	"uniform sampler2D texture0;\n"									
	//					"in vec2 fragmentuv;\n"
	//					"in vec3 fragmentnormal;\n"
	//					"in vec3 lightvec;\n"
	//					"out vec4 color;\n"
	//					"void main(void) {\n"
	//					"color = max(dot(normalize(fragmentnormal),normalize(lightvec)),0) * texture(texture0,fragmentuv);\n"
	//					"}\n";

	//shader->VertexSource(vs);
	//shader->FragmentSource(fs);

	//shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	//shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);
	//shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	//shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);

	//shader->BindAttribLocation(sysattribs[VA_NEXT_POSITION].name,VA_NEXT_POSITION);
	//shader->BindAttribLocation(sysattribs[VA_NEXT_TEXCOORD0].name,VA_NEXT_TEXCOORD0);
	//shader->BindAttribLocation(sysattribs[VA_NEXT_NORMAL].name,VA_NEXT_NORMAL);
	//shader->BindAttribLocation(sysattribs[VA_NEXT_TANGENT].name,VA_NEXT_TANGENT);

	//shader->Compile();

	////shader->Bind();


	////shader->UnBind();

	//return shader;
}

AXELYNX_API axelynx::Shader * axelynx::StandartShaders::Render::Paralax()
{
	static axelynx::Shader * shader=0;
	if(shader)
		return shader;

	shader = axelynx::Shader::Create();

	const char *vs =	"uniform mat4 modelviewproj;\n"
						"uniform mat4 model;\n"
						"uniform mat3 normalmatrix;\n"
						"uniform	vec3	lightpos;\n"
						"uniform	vec3	eyepos;\n"
						"in vec3 position;\n"
						"in vec3 normal;\n"
						"in vec3 tangent;\n"
						"in vec2 texcoord0;\n"
						"out	vec3 ldir;\n"
						"out	vec3 eyedir;\n"
						"out	vec3 halfdir;\n"
						"out vec2 fragmentuv;\n"
						"void main(void)\n"
						"{\n"
						"	vec3	p = vec3      ( model* vec4(position,1.0) );\n"
						"	vec3	l = normalize ( vec3 ( lightpos ) - p );\n"
						"	vec3	e = normalize ( vec3 ( eyepos   ) - p );\n"
						"	vec3	h = l + e;\n"
						"	vec3	n = normalmatrix * normal;\n"
						"	vec3 t = normalmatrix * tangent;\n"
						"	vec3 b = normalmatrix * cross(normal,tangent);\n"																							// now remap l, and e into tangent space
						"	eyedir = vec3 ( dot ( e, t ), dot ( e, b ), dot ( e, n ) );\n"
						"	ldir = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );\n"
						"	halfdir = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );\n"
						"	gl_Position     = modelviewproj * vec4(position,1.0);\n"
						"	fragmentuv = texcoord0;\n"
						"}\n";

	const char *fs =	"uniform sampler2D texture0;\n"
						"uniform sampler2D texture1;\n"

						"uniform float scale;\n"
						"uniform float specExp;\n"
						"uniform float numSteps;\n"

						"in	vec3 ldir;\n"
						"in	vec3 eyedir;\n"
						"in	vec3 halfdir;\n"
						"in vec2 fragmentuv;\n"
						"out vec4 color;\n"
						"void main (void)\n"
						"{\n"
						"	float	step   = 1.0 / numSteps;\n"
						"	vec2	dtex   = eyedir.xy * scale / ( numSteps * eyedir.z );\n"
						"	float	height = 1.0;\n"
						"	vec2	tex    = fragmentuv;\n"
						"	float	h      = texture ( texture1, tex ).a;\n"
						"	while ( h < height )\n"
						"	{\n"
						"		height -= step;\n"
						"		tex    += dtex;\n"
						"		h       = texture ( texture1, tex ).a;\n"
						"	}\n"
						"	vec2	prev   = tex - dtex;\n"
						"	float	hPrev  = texture ( texture1, prev ).a - (height + step);\n"
						"	float	hCur   = h - height;\n"
						"	float	weight = hCur / (hCur - hPrev );\n"
						"	tex = weight * prev + (1.0 - weight) * tex;\n"
						"	vec3	diffuse = texture ( texture0, tex ).rgb;\n"
						"	vec3	n     = normalize ( texture2D ( texture1, tex ).rgb * 2.0 - vec3 ( 1.0 ) );\n"
						"	float	diff  = max ( dot ( n, normalize ( ldir ) ), 0.0 );\n"
						"	float specvalue = max ( dot ( n, normalize ( halfdir ) ), 0.0 );\n"
						"	float	spec  = pow ( specvalue, specExp );\n"
						"	color = vec4 ( diffuse * diff + vec3 ( 0.5 * spec ), 1.0 );\n"
						"}\n";


	shader->VertexSource(vs);
	shader->FragmentSource(fs);

	shader->BindAttribLocation(sysattribs[VA_POSITION].name,VA_POSITION);
	shader->BindAttribLocation(sysattribs[VA_NORMAL].name,VA_NORMAL);
	shader->BindAttribLocation(sysattribs[VA_TANGENT].name,VA_TANGENT);
	shader->BindAttribLocation(sysattribs[VA_TEXCOORD0].name,VA_TEXCOORD0);

	shader->Compile();

	shader->SetUniform("lightpos",axelynx::vec3(0,0,0));
	shader->SetUniform("eyepos",axelynx::vec3(0,0,0));

	shader->SetUniform("scale",-0.05f);
	shader->SetUniform("specExp",80.0f);
	shader->SetUniform("numSteps",16.0f);

	shader->SetUniform(axelynx::Shader::SU_LIGHTPOS,axelynx::vec3(-100,200,-400));

	return shader;
}