#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Normal;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiNormal;
 
void main(void) {
    // Pass through our original color with full opacity.
    //gl_FragColor = vec4(vNormal,1.0);
	vec4 nTex = texture2D(Normal, vTexCoord) * 2 - 1;
	//nTex.b = sqrt(1-pow(length(nTex.ag),2));
	vec3 N = nTex.a * vTangent + -nTex.g * vBiNormal + nTex.b * vNormal;
	
	gl_FragColor.rgb = N * 0.5 + 0.5;
	gl_FragColor.a = 1;
}