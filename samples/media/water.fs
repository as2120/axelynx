uniform sampler2D texture0; //normal
uniform sampler2D texture1; //diffuse
uniform sampler2D texture2; //refraction
uniform sampler2D texture3; //refraction
uniform sampler2D texture4; //z buff scene

uniform vec2 screen_size;

in vec2 fragmentuv;
in vec3 f_mw_normal;

uniform float zFar;
uniform float zNear;

out vec4 color;

void main (void)
{
    vec2 uv = texture(texture0,fragmentuv).xy;
    vec2 duv = uv * 2.0 - vec2(1.0);
    //uv = normalize(uv);




    float zEyeWorld     = (zFar * zNear / ( zFar - texture(texture4,gl_FragCoord.xy / screen_size).x * (zFar - zNear) ));
    float zEyeWater     = (zFar * zNear / ( zFar - gl_FragCoord.z * (zFar - zNear) ));
    float fall = clamp((zEyeWorld - zEyeWater) * 0.5,0.0,4.0);

    float frenel = clamp(f_mw_normal.y * f_mw_normal.y - sqrt((duv.x*duv.x) + (duv.y * duv.y)),0.0,1.0);

    vec4 diff_reflect = texture(texture1,uv);
    vec4 diff_refract = texture(texture2,gl_FragCoord.xy / screen_size + duv * 0.1);

    diff_refract = mix(diff_refract,vec4(0.1,0.25,0.37,1.0),fall);

	color = mix(diff_refract,diff_reflect,frenel*min(1.0,fall));
//color = vec4(z_dist);
}
