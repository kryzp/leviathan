#version 330 core

/* single-pass blur shader (yes ik it runs in O(n^2) not O(n) unlike double-pass but it's simpler this way) */

// https://www.shadertoy.com/view/Xltfzj

out vec4 frag_colour;

in vec2 frag_coord;
in vec4 frag_mod_colour;
in vec4 frag_mode;

uniform sampler2D lev_texture_0;
#define u_texture lev_texture_0

void main()
{
    float Tau = 6.28318530718; // Pi*2

    // GAUSSIAN BLUR SETTINGS {{{
    float Directions = 32.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    float Quality = 32.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
    float Size = 32.0; // BLUR SIZE (Radius)
    // GAUSSIAN BLUR SETTINGS }}}

    vec2 Radius = vec2(Size) / textureSize(u_texture, 0).xy;

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = frag_coord;
    // Pixel colour
    vec4 Color = texture(u_texture, uv);

    // Blur calculations
    for( float d=0.0; d<Tau; d+=Tau/Directions)
    {
        for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
            Color += texture( u_texture, uv+vec2(cos(d),sin(d))*Radius*i);
        }
    }

    // Output to screen
    Color /= Quality * Directions - 15.0;
    frag_colour = Color;
}
