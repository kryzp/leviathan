#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_colour;
layout (location = 3) in vec4 a_mode;

out vec2 frag_coord;
out vec4 frag_mod_colour;
out vec4 frag_mode;

uniform mat4 lev_projection;

void main()
{
	gl_Position = lev_projection * vec4(a_pos, 0.0, 1.0);
	frag_mod_colour = a_colour;
	frag_coord = a_uv;
	frag_mode = a_mode;
}
