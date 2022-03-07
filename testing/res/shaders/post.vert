#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_colour;
layout (location = 2) in vec2 a_tex_coord;

out vec3 colour;
out vec2 texcoord;

uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(a_pos, 1.0);
    colour = a_colour;
    texcoord = a_tex_coord;
}
