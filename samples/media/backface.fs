in	vec3 fragmentuvw;
out vec4 color;

void main (void)
{
	color = vec4 ( fragmentuvw, 1.0 );
}
