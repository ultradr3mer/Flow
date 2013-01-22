#version 130
 
in float vAlpha;
out vec4 gl_FragColor;

uniform float Alpha;
uniform sampler2D Diffuse;
uniform float MaxTexture;
in float vTexture;
 
void main(void) {
    // Pass through our original color with full opacity.
    // gl_FragColor = vec4(vTexCoord.x,vTexCoord.y,0.0,1.0);
	vec4 coord = gl_TexCoord[0];
	coord.x = (coord.x+vTexture)/MaxTexture;
	gl_FragColor.rgb = vec3(1.0,1.0,1.0);
	gl_FragColor.a = vAlpha * texture2D(Diffuse, coord.xy).r;
}