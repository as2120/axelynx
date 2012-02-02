uniform mat4 modelviewproj;
uniform mat3 normalmatrix;
uniform mat4 model;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_NORMAL) in vec3 normal;
layout(location=VA_TANGENT) in vec3 tangent;
layout(location=VA_TEXCOORD0) in vec2 texcoord0;

out vec3 t;
out vec3 b;
out vec3 n;

out vec2 fragmentuv;

out vec3 pos;

void main(void) {
	gl_Position   = modelviewproj * vec4(position,1.0);
	fragmentuv =texcoord0;
	fragmentuv.y *= 50;

	n = normalize(normalmatrix * normal);
	t = normalize(normalmatrix * tangent);
	b = cross(n, t);

	pos = (model * vec4(position,1.0)).xyz;
}
 