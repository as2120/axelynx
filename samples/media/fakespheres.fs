uniform samplerCube texture0;
uniform samplerCube texture1;
uniform sampler2D texture2;
in vec4 fragmentcolor;
in vec2 fragmentuv;
in vec3 fragmenteye;
out vec4 color;

void main(void) {

vec2 r2d = fragmentuv - vec2(0.5,0.5);
float radius = length(r2d);

if(radius>0.5)
	discard;

vec3 normal = vec3(r2d*2.0,0);
normal.z = (0.5 - radius)*2.0;
normal = normalize(normal);


vec3 diffuse = texture(texture0,-normalize(fragmenteye + normal)).rgb;
vec3 bump = (texture(texture1,-normalize(fragmenteye + normal)).xyz)*2.0 - vec3(1.0);
///normal = normalize((bump*2.0)-vec3(1.0) + normal);
normal = normalize(fragmenteye + normal + bump);
float light = clamp(dot(normal,normalize(vec3(-1.0,0.5,0.7))),0.0,1.0);

color = vec4(diffuse * light,1.0);
//color = vec4(fragmenteye*0.5 +vec3(0.5),1.0);
//color = vec4(normal*0.5 +vec3(0.5),1.0);
}
 