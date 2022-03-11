#version 330 core

out vec4 frag_colour;

in vec2 in_uv;
in vec3 in_mod_colour;

uniform sampler2D u_texture;

// just shamelessly stole this shader for the blur
// https://www.shadertoy.com/view/Xltfzj

const float TAU = 6.28318530718;
const float DIRS = 32.0;
const float QUALITY = 8.0;
const float SIZE = 32.0;
const vec2 RESOLUTION = vec2(1280, 720);

void main() {
    vec2 radius = SIZE / RESOLUTION;
    vec4 colour = texture(u_texture, in_uv);
    
    for(float theta = 0.0; theta < TAU; theta += TAU/DIRS) {
	    for(float i = 1.0/QUALITY; i <= 1.0; i += 1.0/QUALITY) {
            vec2 offset = vec2(cos(theta), sin(theta)) * radius * i;
		    colour += texture(u_texture, in_uv + offset);
        }
    }
    
    colour /= QUALITY*DIRS - 15.0;
    frag_colour = colour;
}
