uniform mat4 modelviewproj;

layout(location=VA_POSITION) in vec3 position;

out	vec3 fragmentuvw;

void main(void)
{
	gl_Position     = modelviewproj * vec4(position,1.0);
	fragmentuvw = position * 0.5 + vec3(0.5);
}
