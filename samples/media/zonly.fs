in vec2 fragmentuv;
out vec4 color;

void main(void) {

vec2 r2d = fragmentuv - vec2(0.5,0.5);
float radius = length(r2d);

if(radius>0.5)
	discard;
	color = vec4(1);
}
 