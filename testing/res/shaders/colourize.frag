#version 330 core

out vec4 frag_colour;

in vec2 frag_coord;
in vec4 frag_mod_colour;
in vec4 frag_mode;

uniform sampler2D lev_texture_0;

#define u_texture lev_texture_0

float brightness(vec3 col)
{
	return (0.299*col.r) + (0.587*col.g) + (0.114*col.b);
}

float weight(float x)
{
	return pow(x, 2.0);
}

// https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	vec3 texcol = texture(u_texture, frag_coord).rgb;

	float grayscale = brightness(texcol);

	//frag_colour = vec4(hsv2rgb(vec3(grayscale, 1.0, 1.0)), grayscale);

	// teal->dark blue
	vec3 col_a = vec3(0.1, 1.0, 0.75);
	vec3 col_b = vec3(0.35, 0.1, 0.75);

	// yellow->red
//	vec3 col_a = vec3(1.0, 0.85, 0.6);
//	vec3 col_b = vec3(0.55, 0.15, 0.7);

	////////////////////////////////////////////////////
	
	vec3 final = mix(col_b, col_a, weight(grayscale));
	frag_colour = vec4(final, grayscale);
}
