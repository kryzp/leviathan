#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_colour;

out vec2 in_uv;
out vec3 in_colour;

uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * vec4(a_pos, 0.0, 1.0);
	in_uv = a_uv;
	in_colour = a_colour;
}
