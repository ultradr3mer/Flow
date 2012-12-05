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
	vec3 N = normalize(nTex.a * vTangent + -nTex.g * vBiNormal + nTex.b * vNormal);
	
	vec3 light = vec3(1.1,1.0,0.9);
	vec3 ambient = vec3(0.1,0.1,0.15);
	
	vec3 illumination = clamp(dot(vec3(1,1,1),N),0.0,1.0)*light+ambient;
	gl_FragColor.rgb = texture2D(Diffuse, vTexCoord).rgb*illumination;
	gl_FragColor.a = 1;
}