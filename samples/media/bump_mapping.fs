uniform sampler2D texture0; //diffuse
uniform sampler2D texture1; //normals
uniform samplerCube texture2;
uniform float lightRadius;
uniform float specular;
in vec3 lightVec;
in vec3 eyeVec;
in vec2 fragmentuv;
in vec3 shadow;
in float lightdist;
out vec4 color;

float CalcLighting(int samples, float radius, float step)
{
	float shading = 0;
	for(int i = -samples;i<=samples;++i)
	{
	for(int j = -samples;j<=samples;++j)
	{
	float  att = texture( texture2, vec3(shadow.x,-shadow.y,-shadow.z) + vec3(i*step,0,j*step)).r;
	shading += ( (att+0.5) > (length(shadow.xyz)) ) ? 1.0 : 0.2;
	}
	}

	shading /= float((samples*2+1)*(samples*2+1));
	return shading;
}

void main(void) {
float distSqr = dot(lightVec, lightVec);
float att = clamp(1.0 - lightRadius * sqrt(distSqr), 0.0, 1.0);
vec3 lVec = lightVec * inversesqrt(distSqr);
vec3 vVec = normalize(eyeVec);
vec4 base = texture(texture0, fragmentuv);
vec3 bump = normalize( texture2D(texture1, fragmentuv).xyz * 2.0 - 1.0);
float diffuse = max( dot(lVec, bump), 0.0 );
float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), 8.0 );
color = ((diffuse + specular)*base) * att  * CalcLighting(2,2000.0,lightdist * 0.005);
}
 