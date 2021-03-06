#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;
in float in_Alpha;

uniform mat4 ModelView;
uniform mat4 ViewProjection;
uniform float Aspect;
uniform float Size;
 
out float vAlpha;
void main(void) {

	gl_Position = ViewProjection * ModelView * vec4(in_Position,1);
		
	gl_PointSize = Size / gl_Position.w;
  
    vAlpha = in_Alpha;
	
	gl_TexCoord[0]=gl_MultiTexCoord0;
}