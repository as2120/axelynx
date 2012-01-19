uniform mat4 modelviewproj;
uniform mat3 normalmatrix;
uniform vec3 lightpos;
uniform mat4 model;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_NORMAL) in vec3 normal;

out vec3 fragmentn;
out vec3 lightvec;
out vec3 shadow;

void main(void) {
	gl_Position   = modelviewproj * vec4(position,1.0);
	fragmentn = normalmatrix * normal;
	lightvec = lightpos - (model * vec4(position,1.0)).xyz;
	shadow = (model * vec4(position,1.0)).xyz - lightpos;
}
 