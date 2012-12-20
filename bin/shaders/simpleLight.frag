#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D DefferedLightmap;
uniform vec2 RenderSize;

void main(void) {
	vec2 screenPos = gl_FragCoord.xy / RenderSize;
	
	vec3 illumination = texture2D(DefferedLightmap, screenPos).rgb;
	gl_FragColor.rgb = texture2D(Diffuse, vTexCoord).rgb*illumination;
	gl_FragColor.a = 1;
}