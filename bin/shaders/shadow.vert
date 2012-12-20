#version 130
in vec3 in_Position;

uniform mat4 ModelView;
uniform mat4 ViewProjection;

void main(void) {
    gl_Position = ViewProjection * ModelView * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}