uniform sampler2D texture0; //diffuse
uniform sampler2D texture1; //normals

in vec3 pos;

in vec3 t;
in vec3 b;
in vec3 n;
in vec2 fragmentuv;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 worldpos;

void main(void) {

    color = texture2D(texture0, fragmentuv);
    vec3 tmpnormal = (texture2D(texture1, fragmentuv).rgb * 2.0) - vec3(1.0);
    vec3 tmpnormal2 = vec3(0);

	tmpnormal2.x = dot(tmpnormal, t);
	tmpnormal2.y = dot(tmpnormal, b);
	tmpnormal2.z = dot(tmpnormal, n);

    tmpnormal2 *= 0.5;
    tmpnormal2 += 0.5;

    normal = vec4(tmpnormal2,texture2D(texture1, fragmentuv).a);

    worldpos = vec4(pos,0);
}
 