uniform mat4 modelviewproj;
uniform mat4 modelview;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_NORMAL) in vec3 normal;
layout(location=VA_TEXCOORD0) in vec2 texcoord0;

out vec2 fragmentuv;
out vec3 f_mw_normal;

void main(void)
{
	fragmentuv = texcoord0 * 40.0;
	gl_Position     = modelviewproj * vec4(position,1.0);
	f_mw_normal = (modelview * vec4(normal,0.0)).xyz;
}
