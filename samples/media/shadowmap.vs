uniform mat4 modelviewproj;
uniform mat4 model;
uniform vec3 lightpos;

layout(location=VA_POSITION) in vec3 position;

out vec3 lightdist;
void main(void) {
gl_Position   = modelviewproj * vec4(position,1.0);
lightdist = (model * vec4(position,1.0)).xyz - lightpos;
}
 