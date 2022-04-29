#version 330 core

out vec4 frag_colour;

in vec2 frag_coord;
in vec4 frag_mod_colour;
in vec4 frag_mode;

uniform sampler2D lev_texture_0;

float brightness(vec3 col)
{
	return (0.299*col.r) + (0.587*col.g) + (0.114*col.b);
}

float weight(float x)
{
	return pow(x, 2.0);
}

void main()
{
	vec3 texcol = texture(lev_texture_0, frag_coord).rgb;

	float grayscale = brightness(texcol);

	// teal->dark blue
	vec3 col_a = vec3(0.1, 1.0, 0.75);
	vec3 col_b = vec3(0.35, 0.1, 0.75);

	// yellow->red
	//vec3 col_a = vec3(1.0, 0.85, 0.6);
	//vec3 col_b = vec3(0.55, 0.15, 0.7);

	////////////////////////////////////////////////////
	
	vec3 final = mix(col_b, col_a, weight(grayscale));
	frag_colour = vec4(final, grayscale);
}
