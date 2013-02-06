#version 130
 
in  vec2 vTexCoord;
out vec4 gl_FragColor;

uniform sampler2D Diffuse;
 
void main(void) {
	gl_FragColor = texture2D(Diffuse, vTexCoord);
}