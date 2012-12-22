#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;

uniform int BlurSize = 4;
 
void main(void) {
	vec3 outVal = vec3(0,0,0);
	
	vec2 rastersize = 1.0 / textureSize(Diffuse,0);
	
	for(int i = -BlurSize; i <= BlurSize; i++)
	{
		outVal += texture2D(Diffuse, vec2(vTexCoord.x+rastersize.x*i,vTexCoord.y+rastersize.y*i)).rgb;
	}
	for(int i = -BlurSize; i <= BlurSize; i++)
	{
		outVal += texture2D(Diffuse, vec2(vTexCoord.x+rastersize.x*i,vTexCoord.y-rastersize.y*i)).rgb;
	}
	outVal /= BlurSize*4+2;
		
	gl_FragColor = vec4(outVal,1);
}