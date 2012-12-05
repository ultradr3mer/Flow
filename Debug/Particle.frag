#version 130
 
in float vAlpha;
out vec4 gl_FragColor;

uniform float Alpha;
uniform sampler2D Diffuse;
 
void main(void) {
    // Pass through our original color with full opacity.
    // gl_FragColor = vec4(vTexCoord.x,vTexCoord.y,0.0,1.0);
	vec4 coord = gl_TexCoord[0];
	gl_FragColor = texture2D(Diffuse, coord.xy);
	gl_FragColor.rgb *= vAlpha;
}