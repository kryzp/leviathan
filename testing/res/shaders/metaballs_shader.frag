#version 330 core

out vec4 frag_colour;

in vec2 frag_coord;
in vec4 frag_mod_colour;
in vec4 frag_mode;

uniform sampler2D lev_texture_0;
#define u_texture lev_texture_0

void main()
{
    vec3 col = texture(u_texture, frag_coord).rgb;

    float avg = (col.r + col.g + col.b) / 3.0;

    vec3 final_col = vec3(0.0);

    if (avg >= 0.5)
        final_col = vec3(1.0);

    frag_colour = vec4(final_col, 1.0);
}
