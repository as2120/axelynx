uniform sampler2D texture0; //diffuse
uniform sampler2D texture1; //normals
uniform sampler2D texture2; //world positions
uniform float lightsize;

out vec4 color;

in vec3 l_pos;
in vec3 l_dir;

void main(void) {

vec2 screen_uv = gl_FragCoord.xy /vec2(800.0,600.0);
//screeen_uv.y = 1.0 - screeen_uv.y;

    vec3 l_wpos = l_pos + lightsize * normalize(l_dir);
    vec3 f_wpos = texture(texture2,screen_uv).xyz;

    vec4 diff = texture(texture0,screen_uv);
    vec3 f_wnormal = (texture(texture1,screen_uv).xyz * 0.5) - vec3(0.5);

    float nDotL = max(dot(f_wnormal,normalize(l_dir)),0.0);

    float att = clamp((lightsize - length(l_pos - f_wpos))/lightsize,0.0,1.0);

    color = diff * ((att+0.1) * (nDotL+0.1));
   // color = diff * length(clamp(lightsize/(l_pos - f_wpos),0.0,1.0));
}
 