uniform mat4 modelviewproj;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_TEXCOORD0) in vec2 texcoord0;


out vec2 fragmentuv;


void main(void) {

	gl_Position   = modelviewproj * vec4(position,1.0);
	fragmentuv =texcoord0;
}
 