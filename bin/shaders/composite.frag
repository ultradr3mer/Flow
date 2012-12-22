#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Diffuse2;

uniform float Gamma = 4.0;
uniform float Mult = 1.5;

uniform int BlurSize = 8;
 
void main(void) {
	vec3 outVal = vec3(0,0,0);
	
	outVal = texture2D(Diffuse, vTexCoord).rgb;
	outVal += texture2D(Diffuse2, vTexCoord).rgb;
	
	gl_FragColor = vec4(outVal,1);
}