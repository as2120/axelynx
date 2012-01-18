uniform sampler2D diffuse;	
in vec2 fragmentuv;
out vec4 color;

void main(void)
{
	color = texture2D(diffuse,fragmentuv);
}