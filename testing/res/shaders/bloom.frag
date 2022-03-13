#version 330 core

// https://learnopengl.com/Advanced-Lighting/Bloom

out vec4 frag_colour;
in vec2 frag_coord;
in vec3 frag_mod_colour;

uniform sampler2D u_texture_0; // main scene
uniform sampler2D u_texture_1; // bloom texture

void main() {
    vec3 hdr_col = texture(u_texture_0, frag_coord).rgb;
    vec3 bloom_col = texture(u_texture_1, frag_coord).rgb;

    hdr_col += bloom_col * 4.0;

    vec3 final = vec3(1.0) - exp(-hdr_col * 0.9);
    frag_colour = vec4(final, 1.0) * vec4(frag_mod_colour, 1.0);
}
