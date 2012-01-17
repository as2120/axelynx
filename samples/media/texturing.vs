uniform mat4 modelviewproj;
uniform mat3 normalmatrix;
uniform vec3 lightpos;
uniform mat4 model;
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=4) in vec2 texcoord0;
out vec3 fragmentn;
out vec2 fragmentuv;
out vec3 lightvec;
out vec3 shadow;
void main(void) {
gl_Position   = modelviewproj * vec4(position,1.0);
fragmentn = normalmatrix * normal;
lightvec = lightpos - (model * vec4(position,1.0)).xyz;
fragmentuv = texcoord0;
shadow = (model * vec4(position,1.0)).xyz - lightpos;
}
 