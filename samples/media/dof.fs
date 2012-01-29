uniform sampler2D diffuse;
uniform sampler2D depth;
uniform sampler3D dof_tex;
	
in vec2 fragmentuv;
layout(location = 0) out vec4 color;

uniform float focal_distance;
uniform float focal_range;

uniform float zFar;
uniform float zNear;

void main(void)
{
    float d = texture(depth,fragmentuv).x;

    float zEye     = (zFar * zNear / ( zFar - d * (zFar - zNear) ));
    float dist = zEye - focal_distance;

    float blur = 0;
    if(dist<0)
        blur = pow(-dist / focal_distance,3.0);
    else
        blur = dist / focal_range;
    blur = clamp(blur,0.0,1.0);

    vec4 diffuse = mix(texture(diffuse,fragmentuv),texture(dof_tex,vec3(fragmentuv,blur)),clamp(blur * 7.0,0.0,1.0));

    color = diffuse;
}