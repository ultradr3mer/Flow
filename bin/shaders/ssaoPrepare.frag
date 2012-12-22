#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Normal;
uniform sampler2D Depth;
uniform mat4 ViewProjection;

uniform int BlurSize = 4;
uniform float NearZ;
uniform float FarZ;
 
void main(void) {
	vec4 N = texture2D(Normal,vTexCoord);
	N = N * 2 - 1;
	N.a = 0;
	N = ViewProjection * N;
	N.b *= -1;
	
	float depth = texture2D(Depth, vTexCoord).x;
	depth = 2.0 * depth - 1.0;
	depth = (2.0 * NearZ) / (FarZ + NearZ - depth * (FarZ - NearZ));
		
		
	gl_FragColor.rgb = N.rgb * 0.5 + 0.5;
	gl_FragColor.a = depth;
}