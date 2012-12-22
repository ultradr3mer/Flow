#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;

void main(void) {
	float ao = texture2D(Diffuse,vTexCoord).r;
	gl_FragColor = vec4(0,0,0,1-ao);
	//gl_FragColor = vec4(ao*0.5,ao*0.5,ao*0.5,1);
}