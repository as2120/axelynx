uniform mat4 modelviewproj;
uniform mat4 viewproj;
uniform mat4 model;
uniform vec3 eyepos;

uniform sampler2D texture1; //heightmap
uniform sampler2D texture2; //normalmap

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_TEXCOORD0) in vec2 texcoord0;

out vec2 fragmentuv;
out vec3 fragmentnormal;
void main(void)
{
	vec3 newpos = position;

	newpos.y = texture(texture1,texcoord0).r;
	fragmentnormal = texture(texture2,texcoord0).rgb;
	fragmentuv = texcoord0 * 100.0;

	gl_Position     = modelviewproj * vec4(newpos,1.0);
}
