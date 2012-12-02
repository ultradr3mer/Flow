#version 130
 
in  vec2 vTexCoord;
out vec4 gl_FragColor;

uniform float Alpha;
uniform sampler2D Diffuse;
 
void main(void) {
    // Pass through our original color with full opacity.
    // gl_FragColor = vec4(vTexCoord.x,vTexCoord.y,0.0,1.0);
	gl_FragColor = texture2D(Diffuse, vTexCoord) * Alpha;
}