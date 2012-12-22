#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;

uniform mat4 ModelView;
uniform mat4 ViewProjection;

out vec2 vTexCoord;
void main(void) {

    gl_Position = vec4(in_Position, 1.0);

    vTexCoord = in_TexCoord;
}