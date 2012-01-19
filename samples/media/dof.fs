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

    float zEye     = zFar - (zFar * zNear / ( d * (zFar - zNear) - zFar ));

    float blur = abs(zEye)/focal_range ;

    vec4 diffuse = textureLod(diffuse,fragmentuv,blur * 5.0);

	color = vec4(blur,blur,blur,1.0);
    //color = diffuse;
}