#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Normal;
uniform sampler2D Depth;
uniform mat4 ModelView;

uniform int BlurSize = 2;
uniform float NearZ;
uniform float FarZ;
 
void main(void) {
	vec4 N = vec4(0,0,0,0);

	vec2 rastersize = 1.0 / textureSize(Normal,0);
	
	for(int i = -BlurSize; i <= BlurSize; i++)
	{
		N += texture2D(Normal, vec2(vTexCoord.x+rastersize.x*i,vTexCoord.y+rastersize.y*i));
	}
	for(int i = -BlurSize; i <= BlurSize; i++)
	{
		N += texture2D(Normal, vec2(vTexCoord.x+rastersize.x*i,vTexCoord.y-rastersize.y*i));
	}
	N /= BlurSize*4+2;
	
	//vec4 N = texture2D(Normal,vTexCoord);
	N = N * 2 - 1;
	N.a = 0;
	N = ModelView * N;
	N.b *= -1;
	
	float depth = texture2D(Depth, vTexCoord).x;
	if(depth > 0.01)
	{
		gl_FragColor = vec4(0,0,0,1);
	}
	depth = 2.0 * depth - 1.0;
	depth = (2.0 * NearZ) / (FarZ + NearZ - depth * (FarZ - NearZ));
		
		
	gl_FragColor.rgb = N.rgb * 0.5 + 0.5;
	gl_FragColor.a = depth;
}