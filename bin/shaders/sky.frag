#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Diffuse2;
uniform sampler2D Diffuse3;
uniform sampler2D Diffuse4;
uniform sampler2D Diffuse5;
uniform sampler2D Diffuse6;
 
void main(void) {
	vec4 outVal = vec4(0.0,0.0,0.0,0.0);
	int pos = int(vTexCoord.x);
	
	if(pos == 0)
	{
		outVal += texture2D(Diffuse, vTexCoord);
	}
	else if(pos == 1)
	{
		outVal += texture2D(Diffuse2, vTexCoord);
	}
	else if(pos == 2)
	{
		outVal += texture2D(Diffuse3, vTexCoord);
	}
	else if(pos == 3)
	{
		outVal += texture2D(Diffuse4, vTexCoord);
	}
	else if(pos == 4)
	{
		outVal += texture2D(Diffuse5, vTexCoord);
	}
	else if(pos == 5)
	{
		outVal += texture2D(Diffuse6, vTexCoord);
	}
	
	// outVal.r = outVal.a;
	// outVal.a = 1.0;
	
	outVal.rgb *= outVal.a;
	
	gl_FragColor = outVal;
}