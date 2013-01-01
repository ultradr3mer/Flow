#version 130
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_TexCoord;

uniform mat4 ViewProjection;

out vec2 vTexCoord;
out vec3 vView;
void main(void) {

    gl_Position = vec4(in_Position, 1.0);
	
	vec4 close = ViewProjection * vec4(in_Position.x,in_Position.y, -1.0, 1.0);
	close /= close.w;
	vec4 far = ViewProjection * vec4(in_Position.x,in_Position.y, 1.0, 1.0);
	far /= far.w;
	vView = vec3(far - close);

    vTexCoord = in_TexCoord;
}