uniform sampler2D texture0; //diffuse

in vec2 fragmentuv;
in vec3 fragmentnormal;
out vec4 color;

void main (void)
{
    float light = max(0,dot(fragmentnormal,vec3(0,1,0)));

	color = light * texture(texture0,fragmentuv);
}
