uniform mat4 modelviewproj;
uniform float fframe;
layout(location = VA_POSITION) in vec3 position;
layout(location = VA_COLOR) in vec4 color;
layout(location = VA_POINTSIZE) in float pointsize;
out vec4 fragmentcolor;
out float frame;
void main(void) {
gl_Position   = modelviewproj * vec4(position,1.0) ;
gl_PointSize = pointsize * 500.0f / gl_Position.z;
frame = fframe + position.x * 40.0 + position.y * 20.0 + position.z * 7.0 + 1000000.0;
fragmentcolor = color;
}