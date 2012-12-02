#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;

uniform mat4 ModelView;
uniform mat4 ViewProjection;
uniform vec3 Origin;
uniform float Aspect;
uniform float Size;
 
out vec2 vTexCoord;
void main(void) {

	vec4 origin = ViewProjection * ModelView * vec4(Origin,1);

    gl_Position = 
		vec4(
		origin.x + in_Position.x * Size, 
		origin.y - in_Position.z * Size * Aspect, 
		origin.z, 
		origin.w);
  
    vTexCoord = in_TexCoord;
}