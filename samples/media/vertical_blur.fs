uniform sampler2D diffuse;
uniform float delta;

in vec2 fragmentuv;
layout(location = 0) out vec4 color0;
layout(location = 1) out vec4 color1;
layout(location = 2) out vec4 color2;
layout(location = 3) out vec4 color3;
layout(location = 4) out vec4 color4;
layout(location = 5) out vec4 color5;
layout(location = 6) out vec4 color6;
layout(location = 7) out vec4 color7;

void main(void)
{

	color0 = texture(diffuse,fragmentuv);
    color1 = (texture(diffuse,fragmentuv + vec2(0,delta)) + texture(diffuse,fragmentuv - vec2(0,delta)) + color0) / 3.0;
    color2 = (texture(diffuse,fragmentuv + vec2(0,delta * 2.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 2.0)) + color1 * 3.0) / 5.0;
    color3 = (texture(diffuse,fragmentuv + vec2(0,delta * 3.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 3.0)) + color2 * 5.0) / 7.0;
    color4 = (texture(diffuse,fragmentuv + vec2(0,delta * 4.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 4.0)) + color3 * 7.0) / 9.0;
    color5 = (texture(diffuse,fragmentuv + vec2(0,delta * 5.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 5.0)) + color4 * 9.0) / 11.0;
    color6 = (texture(diffuse,fragmentuv + vec2(0,delta * 6.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 6.0)) + color5 * 11.0) / 13.0;
    color7 = (texture(diffuse,fragmentuv + vec2(0,delta * 7.0)) + texture(diffuse,fragmentuv - vec2(0,delta * 7.0)) + color6 * 13.0) / 15.0;

//color0 = vec4(1,0,0,1);
//color1 = vec4(0,1,0,1);
//color2 = vec4(0,0,1,1);
//color3 = vec4(1,1,0,1);
//color4 = vec4(1,0,1,1);
//color5 = vec4(0,0.6,1,1);
//color6 = vec4(0.2,0.4,0,1);
//color7 = vec4(0.7,1,1,1);

}