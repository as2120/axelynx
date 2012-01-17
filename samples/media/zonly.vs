uniform mat4 modelviewproj;
layout(location=0) in vec3 position;
layout(location=3) in vec4 color;
layout(location=8) in float pointsize;
out float size;
void main(void)
{
gl_Position  = modelviewproj * vec4( position, 1.0 );
size         = pointsize;
}