uniform sampler2D texture0; //normal
uniform sampler2D texture1; //diffuse

in vec2 fragmentuv;
out vec4 color;

void main (void)
{
    vec2 uv = texture(texture0,fragmentuv).xy;
    //uv = normalize(uv);
	color = texture(texture1,uv);
}
