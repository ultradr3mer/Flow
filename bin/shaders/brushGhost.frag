#version 130
 
in  vec2 vTexCoord;
out vec4 gl_FragColor;

uniform sampler2D Diffuse;

in vec3 vNormal;

float texture()
{
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	
	x -= x / 4 * 4;
	y -= y / 4 * 4;
	
	return x == y ? 0.2 : 0.7;
}

void main(void) {
	vec3 N = vNormal;
	
	N.x *= sign(N.x);
	N.y *= sign(N.y);
	N.z *= sign(N.z);
	
	N = N * 0.4 + 0.2;
	
	gl_FragColor.rgb = N;
	gl_FragColor.a = texture();
}