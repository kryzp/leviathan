#version 330 core

out vec4 frag_colour;
  
in vec3 colour;
in vec2 texcoord;

uniform sampler2D u_texture;
uniform int u_time;

void main()
{
    float offsetx = cos(u_time / 50.0 + texcoord.y*25.0) * 0.1;
    float offsety = sin(u_time / 50.0 + texcoord.x*25.0) * 0.1;
    vec2 uv = vec2(texcoord.x + offsetx, texcoord.y + offsety);
    vec4 texcol = texture(u_texture, uv);
    frag_colour = texcol * vec4(colour, 1.0);
}
