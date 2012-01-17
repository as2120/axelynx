uniform sampler3D texture0;
uniform sampler2D texture1;

in	vec3 fragmentuvw;
out vec4 color;

void main (void)
{
    vec3 start = fragmentuvw;
    vec3 end = texture(texture1,gl_FragCoord.xy / vec2(800.0,600.0)).rgb;

    const float numsteps = 128.0;
    const float step = 1.0 / numsteps;

    float sat = 0.0;

    for(float s =0.0;s<1.0;s+=step)
    {
           float r = texture(texture0,mix(start,end,s)).r;
        sat += r*r ;
    };

    sat /= numsteps;

	color = vec4 ( vec3(sat*4.0), 1.0 );
}
