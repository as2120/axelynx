uniform sampler2D texture0; //YUV2

uniform float video_width;
uniform float video_height;

in vec2 fragmentuv;
out vec4 color;

void main(void) {

    float y = texture(texture0,fragmentuv).r;
		
    ivec2 texelpos = ivec2(fragmentuv.x * video_width,fragmentuv.y * video_height);

    int rowindex = int(fragmentuv.x * video_width);

    float u = 0.0;
    float v = 0.0;

    if(rowindex % 2 == 0)
    {
	    u = texelFetch(texture0, texelpos,0).a - 0.5;
	    v = texelFetch(texture0, texelpos + ivec2(1,0),0).a - 0.5;
	}
    else
    {		 
	    u = texelFetch(texture0, texelpos - ivec2(1,0),0).a - 0.5;
	    v = texelFetch(texture0, texelpos,0).a - 0.5;
	}

	float r = y+1.371*(v);
	float g = y-0.698*(v)-0.336*u;
	float b = y+1.732*(u);

    color = vec4(r,g,b,1);
}
 