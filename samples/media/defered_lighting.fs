uniform sampler2D texture0; //diffuse
uniform sampler2D texture1; //normals

uniform float lightRadius;
in vec3 lightVec;
in vec3 eyeVec;
in vec2 fragmentuv;
in vec3 shadow;
in float lightdist;
out vec4 color;

void main(void) {
float distSqr = dot(lightVec, lightVec);
float att = clamp(1.0 - lightRadius * sqrt(distSqr), 0.0, 1.0);
vec3 lVec = lightVec * inversesqrt(distSqr);
vec3 vVec = normalize(eyeVec);
vec4 base = texture(texture0, fragmentuv);
vec3 bump = normalize( texture2D(texture1, fragmentuv).xyz * 2.0 - 1.0);
float diffuse = max( dot(lVec, bump), 0.0 );
float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), texture2D(texture1, fragmentuv).a * 12.0);
color = vec4(1.0,1.0,1.0,1.0);
}
 