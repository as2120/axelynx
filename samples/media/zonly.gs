layout (points) in;
layout (triangle_strip, max_vertices = 6) out;
uniform vec3 viewup;
uniform vec3 viewright;
in float size [];
out vec2 fragmentuv;
void main ()
{
vec3	u = viewup * size [0];
vec3	r = viewright * size [0];
vec3	p = gl_in [0].gl_Position.xyz;
float	w = gl_in [0].gl_Position.w;

gl_Position = vec4 ( p - u - r, w );
fragmentuv = vec2(0,0);
EmitVertex ();
gl_Position = vec4 ( p - u + r, w );
fragmentuv = vec2(1,0);
EmitVertex ();
gl_Position = vec4 ( p + u + r, w );
fragmentuv = vec2(1,1);
EmitVertex   ();
EndPrimitive ();
gl_Position = vec4 ( p + u + r, w );
fragmentuv = vec2(1,1);
EmitVertex   ();
gl_Position = vec4 ( p + u - r, w );
fragmentuv = vec2(0,1);
EmitVertex ();
gl_Position = vec4 ( p - u - r, w );
fragmentuv = vec2(0,0);
EmitVertex   ();
 EndPrimitive ();
}