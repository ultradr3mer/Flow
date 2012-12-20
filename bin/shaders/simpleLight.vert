#version 130
in vec3 in_Position;
in vec2 in_TexCoord;

uniform mat4 ModelView;
uniform mat4 ViewProjection;
 
out vec2 vTexCoord;
void main(void) {
	vTexCoord = in_TexCoord;
 
    gl_Position = ViewProjection * ModelView * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}