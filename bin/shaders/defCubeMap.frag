#version 130
 
in  vec2 vTexCoord;
//out vec4 gl_FragColor;

uniform sampler2D Diffuse;
uniform sampler2D Diffuse2;
uniform sampler2D Diffuse3;
uniform sampler2D Diffuse4;
uniform sampler2D Diffuse5;
uniform sampler2D Diffuse6;

uniform sampler2D Normal;

in vec3 vView;

vec3 getRef(vec3 refn){
	vec3 tmpn = refn;
	
	refn.y *= -1;

	tmpn.x *= sign(tmpn.x);
	tmpn.y *= sign(tmpn.y);
	tmpn.z *= sign(tmpn.z);
		
	vec4 env = vec4(0,0,0,1);
	vec2 envuv;
	if(tmpn.z >= tmpn.y && tmpn.z >= tmpn.x){
		if(refn.z < 0){
			envuv = vec2(-refn.x/refn.z,refn.y/refn.z)*0.5+0.5;
			return texture2D(Diffuse,envuv).rgb;
		} else {
			envuv = vec2(-refn.x/refn.z,-refn.y/refn.z)*0.5+0.5;
			return texture2D(Diffuse3,envuv).rgb;
		}
	}
	else if(tmpn.x >= tmpn.y && tmpn.x >= tmpn.z){
		if(refn.x < 0){
			envuv = vec2(refn.z/refn.x,refn.y/refn.x)*0.5+0.5;
			return texture2D(Diffuse2,envuv).rgb;
		} else {
			envuv = vec2(refn.z/refn.x,-refn.y/refn.x)*0.5+0.5;
			return texture2D(Diffuse4,envuv).rgb;
		}
	} 
	else if(tmpn.y >= tmpn.z && tmpn.y >= tmpn.x){
		if(refn.y < 0){
			envuv = vec2(-refn.x/refn.y,-refn.z/refn.y)*0.5+0.5;
			return texture2D(Diffuse5,envuv).rgb;
		} else {
			envuv = vec2(refn.x/refn.y,-refn.z/refn.y)*0.5+0.5;
			return texture2D(Diffuse6,envuv).rgb;
		}
	} 
}

 
void main(void) {
	vec3 outVal = vec3(0,0,0);
	
	vec3 N = texture2D(Normal, vTexCoord).rgb * 2 - 1;
	vec3 refVec = reflect(normalize(vView),N);
	
	outVal = getRef(N);
	//outVal = texture2D(Diffuse3,vTexCoord).rgb;
	
	gl_FragColor = vec4(outVal,0.2);
}