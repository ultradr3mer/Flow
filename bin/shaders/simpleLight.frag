#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D DefferedLightmap;
uniform sampler2D Reflections;
uniform vec2 RenderSize;

void main(void) {
	vec2 screenPos = gl_FragCoord.xy / RenderSize;
	
	
	vec4 illumination = texture2D(DefferedLightmap, screenPos);
	vec3 illColor = illumination.rgb / (illumination.r + illumination.g + illumination.b) * 3;
	
	vec3 Reflection = texture2D(Reflections, screenPos).rgb * 0.2;
	
	gl_FragColor.rgb = texture2D(Diffuse, vTexCoord).rgb*illumination+illColor*illumination.a*0.4+Reflection;
	gl_FragColor.a = 1;
}