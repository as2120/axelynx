uniform sampler3D diffuse;
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
    float step = 1.0 / 7.0;

	color0 = texture(diffuse,vec3(fragmentuv,0));
    color1 = (texture(diffuse,vec3(fragmentuv + vec2(delta,0),step * 1.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta,0),step *  1.0)) + color0) / 3.0;
    color2 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 2.0,0),step * 2.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 2.0,0),step * 2.0)) + color1 + color0) / 4.0;
    color3 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 3.0,0),step * 3.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 3.0,0),step * 3.0)) + color2 + color1 + color0) / 5.0;
    color4 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 4.0,0),step * 4.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 4.0,0),step * 4.0)) + color3 + color2 + color1 + color0) / 6.0;
    color5 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 5.0,0),step * 5.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 5.0,0),step * 5.0)) + color4 + color3 + color2 + color1 + color0) / 7.0;
    color6 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 6.0,0),step * 6.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 6.0,0),step * 6.0)) + color5 + color4 + color3 + color2 + color1 + color0) / 8.0;
    color7 = (texture(diffuse,vec3(fragmentuv + vec2(delta * 7.0,0),step * 7.0)) + texture(diffuse,vec3(fragmentuv - vec2(delta * 7.0,0),step * 7.0)) + color6 + color5 + color4 + color3 + color2 + color1 + color0) / 9.0;
}