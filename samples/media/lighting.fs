uniform samplerCube texture0;

in vec3 fragmentn;
in vec3 lightvec;
in vec3 shadow;
out vec4 color;

float CalcLighting(int samples, float radius, float step)
{
	float shading = 0;
	for(int i = -samples;i<=samples;++i)
	{
	for(int j = -samples;j<=samples;++j)
	{
	float  att = texture( texture0, vec3(shadow.x,-shadow.y,-shadow.z) + vec3(i*step,0,j*step)).r;
	shading += ( (att+0.5) > (length(shadow.xyz)) ) ? 1.0 : 0.2;
	}
	}

	shading /= float((samples*2+1)*(samples*2+1));
	return shading;
}

void main(void) {
float diff = max(dot(normalize(fragmentn),normalize(lightvec)),0.01) * CalcLighting(2,2000.0,length(lightvec) * 0.005);
color = vec4(diff,diff,diff,1.0);
}
 