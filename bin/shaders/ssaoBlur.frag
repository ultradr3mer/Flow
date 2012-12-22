#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Normal;

uniform int BlurSize = 3;
uniform float MaxDelta = 0.3;

vec4 readSample(in vec2 coord)
{
	vec4 sample = texture2D(Normal,coord);
	sample.xy = sample.xy * 2 - 1;
	sample.z = sample.z * -2 + 1;

	return sample;
}
 
void main(void) {
	vec3 outVal = vec3(0,0,0);
	
	vec2 rastersize = 1.0 / textureSize(Normal,0);
	
	vec4 curPixel = readSample(vTexCoord);

	float ao = 0;

	vec4 sample;
	float normalFact;
	float max = 0;
	for(int i = -BlurSize; i < BlurSize; i += 1)
	{
		for(int j = -BlurSize; j < BlurSize; j += 1)
		{
			sample = readSample(vec2(vTexCoord.x+rastersize.x*i,vTexCoord.y+rastersize.y*j));
			
			normalFact = dot(sample.xyz,curPixel.xyz) > MaxDelta ? 1.0 : 0.1;
			
			ao += normalFact * sample.a;
			max += normalFact;
		}
	}
	ao /= max;
		
	gl_FragColor = vec4(ao,ao,ao,1);
}