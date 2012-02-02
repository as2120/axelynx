uniform mat4 modelviewproj;
uniform mat4 model;
uniform mat4 modelview;
uniform mat3 normalmatrix;
uniform vec3 lightpos;

layout(location=VA_POSITION) in vec3 position;
layout(location=VA_NORMAL) in vec3 normal;
layout(location=VA_TANGENT) in vec3 tangent;
layout(location=VA_TEXCOORD0) in vec2 texcoord0;

out vec3 lightVec;
out vec3 eyeVec;
out vec2 fragmentuv;
out vec3 shadow;

out float lightdist;

void main(void) {
	lightdist = length(lightpos - (model * vec4(position,1.0)).xyz);
	gl_Position   = modelviewproj * vec4(position,1.0);
	fragmentuv =texcoord0;
	fragmentuv.y *= 50;

	vec3 n = normalize(normalmatrix * normal);
	vec3 t = normalize(normalmatrix * tangent);
	vec3 b = cross(n, t);
	vec3 vVertex = vec3(model * vec4(position,1));
	vec3 tmpVec = lightpos - vVertex;
	lightVec.x = dot(tmpVec, t);
	lightVec.y = dot(tmpVec, b);
	lightVec.z = dot(tmpVec, n);
	tmpVec = -vec3(modelview * vec4(position,1));
	eyeVec.x = dot(tmpVec, t);
	eyeVec.y = dot(tmpVec, b);
	eyeVec.z = dot(tmpVec, n);
	shadow = (model * vec4(position,1.0)).xyz - lightpos;
}
 