#version 330 core

out vec4 frag_colour;
  
in vec3 in_colour;
in vec2 tex_coord;

uniform sampler2D u_texture;

void main()
{
    vec4 texcol = texture(u_texture, tex_coord);
    frag_colour = texcol * vec4(in_colour, 1.0);
}
