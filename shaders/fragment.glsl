#version 400 core
#define FLAG_NMAP		1u
#define FLAG_NOLIGHT	2u
#define AXIS_Y			vec3(0.0, 1.0, 0.0)
#define AXIS_Z			vec3(0.0, 0.0, 1.0)
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

mat3	get_tbn(vec3 n)
{
	vec3	t;
	vec3	b;

	n = normalize(n);
	t = vec3(n.xy, 0.0f);
	if (length(t) != 0)
		t = normalize(t);
	else
		t = vec3(1.0f, 0.0f, 0.0f);
	t = cross(t, n);
	b = cross(t, n);
	return (mat3(normalize(t), normalize(b), n));
}

vec3	get_normal(void)
{
	vec3	normal;

	if ((flags & FLAG_NMAP) == 0)
	{
		mat3 normal_matrix = transpose(inverse(mat3(model)));
		normal = normal_matrix * fnormal;
		return (normal);
	}
	normal = texture(normal_map, uv).rgb * 2.0 - 1.0;
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	mat3 tbn = get_tbn(normal_matrix * fnormal);
	normal *= tbn;
	return (normalize(normal));
}

float	make_brightness(void)
{
	vec3 normal = get_normal();
	vec3 fpos = vec3(model * vec4(fvertex.xyz, 1));
	//a vector pointing to the light
	//vec3 stl = light.position * mat3(model) - fpos;
	vec3 stl = light.position - fpos;
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
	// color.xyz = (get_normal() + 1.0f) * 0.5;
	frag_color = color;
}
