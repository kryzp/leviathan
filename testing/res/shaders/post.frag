#version 330 core

out vec4 frag_colour;

in vec2 in_uv;
in vec3 in_colour;

uniform sampler2D u_texture;

void main() {
	frag_colour = texture(u_texture, in_uv) * vec4(in_colour, 1.0);
}
