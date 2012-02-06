uniform mat4 modelviewproj;

uniform vec3 lightpos;
uniform float lightsize;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_NORMAL) in vec3 normal;

out vec3 l_pos;
out vec3 l_dir;

void main(void) {

	gl_Position   = modelviewproj * vec4(position,1.0);
	l_dir = normal;
	l_pos = lightpos;
}
 