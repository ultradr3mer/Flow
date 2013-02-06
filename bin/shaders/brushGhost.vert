#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;

uniform mat4 ModelView;
uniform mat4 ViewProjection;

uniform vec3 Size;
 
out vec2 vTexCoord;
out vec3 vNormal;
void main(void) {
    gl_Position = ViewProjection * ModelView * vec4(in_Position * Size, 1.0);

    vTexCoord = in_TexCoord;
	vNormal = in_Normal;
}