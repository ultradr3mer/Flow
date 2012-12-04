#version 130
 
in  vec2 vTexCoord;
out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform float State;
const float PI = 3.1415926535897932384626433832795f;
 
void main(void) {
    // Pass through our original color with full opacity.
    // gl_FragColor = vec4(vTexCoord.x,vTexCoord.y,0.0,1.0);
	vec2 normalizedCoord = vTexCoord - 0.5;
	float raising = atan(normalizedCoord.x,normalizedCoord.y);
	raising += PI;
	raising /= PI*2;
	raising = raising < State ? 0.0 : 1.0;
	
	gl_FragColor = texture2D(Diffuse, vTexCoord);
	gl_FragColor.gb *= raising;
}