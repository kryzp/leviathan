#version 330 core

out vec4 frag_colour;
  
in vec3 in_colour;
in vec2 tex_coord;

uniform sampler2D u_texture;

void main()
{
    frag_colour = texture(u_texture, tex_coord) * vec4(in_colour, 1.0);
}