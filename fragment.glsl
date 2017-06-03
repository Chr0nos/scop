#version 400 core
#define FLAG_NMAP		1u
#define FLAG_NOLIGHT	2u
uniform sampler2D	texture_sampler;
uniform sampler2D	normal_map;
uniform float		tex_switch;
uniform int			tex_mode;
uniform struct		s_light {
	vec3			position;
	vec4			color;
}					light;
uniform mat4		model;
uniform uint		flags;

in vec2				uv;
in vec4				fcolor;
in vec3				fnormal;
in vec4				fvertex;

out vec4			frag_color;

vec3	get_normal(void)
{
	if ((flags & FLAG_NMAP) != 0)
		return (texture(normal_map, uv).rgb * 2.0 - 1.0);
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	return (normalize(normal_matrix * fnormal));
}

float	make_brightness(void)
{
	vec3 normal = get_normal();
	vec3 fpos = vec3(model * vec4(fvertex.xyz, 1));
	//a vector pointing to the light
	vec3 stl = light.position * mat3(model) - fpos;
	float brightness = dot(normal, stl) / (length(stl) * length(normal));
	brightness = clamp(brightness, 0.2, 1.0);
	return (brightness);
}

void main() {
	vec4 color = texture(texture_sampler, uv);
	float brightness;

	if ((flags & FLAG_NOLIGHT) == 0)
		brightness = make_brightness();
	else
		brightness = 1.0;
	color *= light.color * brightness;
	color = mix(color, fcolor, clamp(tex_switch, 0, 1));
	frag_color = color;
}
