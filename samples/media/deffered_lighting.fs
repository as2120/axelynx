uniform sampler2D texture0; //diffuse
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //world positions
uniform float lightsize;
uniform vec2 screen_size;
uniform vec3 eyepos;

out vec4 color;

in vec3 l_pos;
in vec3 l_dir;

void main(void) {

vec2 screen_uv = gl_FragCoord.xy / screen_size;
//screen_uv.y = 1.0 - screen_uv.y;

    vec3 l_wpos = l_pos + lightsize * normalize(l_dir);
    vec3 f_wpos = texture(texture2,screen_uv).xyz;

    vec4 diff = texture(texture0,screen_uv);
    vec3 f_wnormal = (texture(texture1,screen_uv).xyz * 2.0) - vec3(1.0);

    float nDotL = max(dot(f_wnormal,normalize(l_dir)),0.0);

    float att = clamp((lightsize - length(l_pos - f_wpos))/lightsize,0.0,1.0);


    float specular = pow(clamp(dot(reflect(-normalize(l_dir), f_wnormal), normalize(eyepos -f_wpos) ), 0.0, 1.0), 6.0);

    color = diff *  att * (nDotL + specular) * 2.0;
   // color = diff * length(clamp(lightsize/(l_pos - f_wpos),0.0,1.0));
}
 