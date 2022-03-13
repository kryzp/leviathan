#version 330 core

// https://www.shadertoy.com/view/Ms23DR
// https://learnopengl.com/Advanced-Lighting/Bloom

layout (location = 0) out vec4 frag_colour;
layout (location = 1) out vec4 bright_colour;

in vec2 frag_coord;
in vec3 frag_mod_colour;

uniform float u_time;
uniform sampler2D u_texture_0;
uniform vec2 u_resolution;

vec2 curve(vec2 uv, float strength) {
	uv = (uv - 0.5) * 2.0;
	uv *= 1.1;
	uv.x *= 1.0 + pow(abs(uv.y) / 5.0, strength);
	uv.y *= 1.0 + pow(abs(uv.x) / 4.0, strength);
	uv = (uv / 2.0) + 0.5;
	uv = (uv * 0.92) + 0.04;
	return uv;
}

void main() {
	vec2 uv = curve(frag_coord, 2.25);
	vec3 oricol = texture(u_texture_0, frag_coord).rgb;
	vec3 col;
	float x = sin((0.3 * u_time) + (uv.y * 21.0)) * sin((0.7 * u_time) + (uv.y * 29.0)) * sin(0.3 + (0.33 * u_time) + (uv.y * 31.0)) * 0.00017;

	col.r = texture(u_texture_0, vec2(x + uv.x + 0.001, uv.y + 0.001)).r + 0.05;
	col.g = texture(u_texture_0, vec2(x + uv.x + 0.000, uv.y - 0.002)).g + 0.05;
	col.b = texture(u_texture_0, vec2(x + uv.x - 0.002, uv.y + 0.000)).b + 0.05;
	col.r += 0.03 * texture(u_texture_0, (0.75 * vec2(x+0.025, -0.027)) + vec2(uv.x + 0.001, uv.y + 0.001)).r;
	col.g += 0.02 * texture(u_texture_0, (0.75 * vec2(x+-0.022, -0.02)) + vec2(uv.x + 0.000, uv.y - 0.002)).g;
	col.b += 0.03 * texture(u_texture_0, (0.75 * vec2(x+-0.02, -0.018)) + vec2(uv.x - 0.002, uv.y + 0.000)).b;

	col = clamp((col * 0.6) + (col * col * 0.4), 0.0, 1.0);

	float vig = 16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
	float viginp = pow(abs(vig), 0.35);
	col *= vec3(viginp, viginp, viginp);

	col *= vec3(1.01, 1.05, 1.02);
	col *= 3.4;

	float scans = clamp(0.35+0.35*sin((3.5 * u_time) + (uv.y * u_resolution.y * 1.5)), 0.0, 1.0);
	float s = pow(abs(scans), 1.7);
	float sinp = 0.4 + (0.7 * s);
	col *= sinp;

	if (uv.x < 0.0 || uv.x > 1.0)
		col *= 0.0;
	if (uv.y < 0.0 || uv.y > 1.0)
		col *= 0.0;

	col *= 1.0 - (0.65 * clamp((mod(frag_coord.x, 2.0)-1.0) * 2.0, 0.0, 1.0));

	frag_colour = vec4(col.rgb, 1.0) * vec4(frag_mod_colour, 1.0);

	float avg = (col.r + col.g + col.b) / 3.0;
	if (avg >= 0.85)
		bright_colour = vec4(col.rgb, 1.0);
	else
		bright_colour = vec4(0.0, 0.0, 0.0, 0.0);
}
