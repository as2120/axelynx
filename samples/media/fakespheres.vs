uniform mat4 modelviewproj;
layout(location=0) in vec3 position;
layout(location=3) in vec4 color;
layout(location=8) in float pointsize;
out float size;
out vec4 vcolor;
out vec3 vpos;
void main(void)
{
gl_Position  = modelviewproj * vec4( position, 1.0 );
vpos = position;
size         = pointsize;
vcolor         = color;
}