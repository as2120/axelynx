in vec2 position;
uniform mat4 projection;
uniform mat3x2 transform;
in vec2 texcoord;
out vec2 fragmentuv;
vec2 VTransform(vec2 pos, mat3x2 m)
{
return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);
}

void main(void)
{
	vec2 tpos = VTransform(position, transform);
	gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);
	fragmentuv = texcoord;
}

