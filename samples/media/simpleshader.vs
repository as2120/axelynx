uniform mat4 modelviewproj;
uniform float time;

layout(location=0) in vec3 position;

void main(void)
{
	vec3 newpos = position;
	newpos.y = sin(position.x*0.5+time) * cos(position.z-time*0.5);

	gl_Position     = modelviewproj * vec4(newpos,1.0);
}
