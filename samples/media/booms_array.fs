uniform sampler2DArray texture0;		
in vec4 fragmentcolor;
in vec2 fragmentuv;
in float frame;
out vec4 color;
void main(void) {
int iframe0 = int(frame);
int iframe1 =iframe0+1;
float scalar = frame- iframe0;

iframe0 = iframe0 % 40;
iframe1 = iframe1 % 40;
vec4 tex0 = texture(texture0,vec3(gl_PointCoord.x,1.0-gl_PointCoord.y,iframe0));
vec4 tex1 = texture(texture0,vec3(gl_PointCoord.x,1.0-gl_PointCoord.y,iframe1));

color = fragmentcolor * mix(tex1,tex0,scalar);
}