#version 330 core

out vec4 FragColor;
  
in vec3 our_colour;
in vec2 tex_coord;

uniform int u_time;
uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, tex_coord);
}
