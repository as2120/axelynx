uniform sampler2D diffuse;
uniform sampler2D depth;
	
in vec2 fragmentuv;
out vec4 color;

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
        blur = -dist / focal_distance;
    else
        blur = dist / focal_range;
    blur = clamp(blur,0.0,1.0);

    vec4 diffuse = textureLod(diffuse,fragmentuv,blur * 8.0);

	//color = vec4(blur,blur,blur,1.0);
    color = diffuse;
}