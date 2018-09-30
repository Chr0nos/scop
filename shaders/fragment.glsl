#version 400 core
#define FLAG_NMAP		1u
#define FLAG_NOLIGHT	2u
#define FLAG_DBG_TAN	4u
#define FLAG_DBG_NORM	8u
#define FLAG_DBG		(FLAG_DBG_TAN | FLAG_DBG_NORM)
#define AXIS_X			vec3(1.0, 0.0, 0.0)
#define AXIS_Y			vec3(0.0, 1.0, 0.0)
#define AXIS_Z			vec3(0.0, 0.0, 1.0)
uniform sampler2D	texture_sampler;
uniform sampler2D	normal_map;
uniform sampler2D	ambiant_occlusion;
uniform float		tex_switch;
uniform int			tex_mode;
uniform struct		s_light {
	vec3			position;
	vec4			color;
	float			intensity;
}					light;
uniform mat4		model;
uniform mat4		view;
uniform uint		flags;

in vec2				uv;
in vec4				fcolor;
in vec3				fnormal;
in vec4				fvertex;
in vec3				ftangeant;

out vec4			frag_color;

mat3	get_tbn(vec3 n)
{
	vec3	t;
	vec3	b;

	n = normalize(n);
	t = ftangeant;
	b = cross(t, n);
	return (mat3(normalize(t), normalize(b), n));
}

vec3	get_normal(void)
{
	vec3	normal;

	if ((flags & FLAG_NMAP) == 0)
	{
		mat3 normal_matrix = transpose(inverse(mat3(model)) * mat3(view));
		normal = normal_matrix * fnormal;
		return (normal);
	}
	normal = texture(normal_map, uv).rgb * 2.0 - 1.0;
	mat3 normal_matrix = transpose(inverse(mat3(model)) * mat3(view));
	mat3 tbn = get_tbn(normal_matrix * fnormal);
	normal *= tbn;
	return (normalize(normal));
}

float	make_brightness(void)
{
	vec3 normal = get_normal();
	//vec3 fpos = vec3((inverse(model) * view) * vec4(fvertex.xyz, 1));
	vec3 fpos = vec3(transpose(view) * vec4(fvertex.xyz, 1));
	//a vector pointing to the light
	//vec3 stl = light.position * mat3(model) - fpos;
	vec3 stl = light.position - fpos;
	//float brightness = dot(normal, stl) / (length(stl) * length(normal));
	float brightness = dot(normal, normalize(stl));
	brightness = clamp(brightness, 0.2, 1.0);
	return (brightness);
}

float	make_directional_brightness(vec3 direction)
{
	float brightness = max(dot(get_normal(), direction), 0);
	return (clamp(brightness, 0.0, 1.0));
}

void main() {
	vec4 color = texture(texture_sampler, uv);

	if ((flags & FLAG_DBG_NORM) != 0)
		color.xyz = (vec4(fnormal, 0) * inverse(model) * view).xyz;
	if ((flags & FLAG_DBG_TAN) != 0)
		color.xyz = (vec4(ftangeant, 0) * inverse(model) * view).xyz;
	if ((flags & FLAG_DBG) == 0)
	{
		if ((flags & FLAG_NOLIGHT) == 0)
		{
			color *= light.color *
				//make_directional_brightness(AXIS_Z);
				make_brightness() * ((texture(ambiant_occlusion, uv) + 2.0) / 3.0);
		}
		if (tex_switch > 0)
			color = mix(color, fcolor, clamp(tex_switch, 0, 1));
	}
//	vec3 fpos = vec3(transpose(view) * vec4(fvertex.xyz, 1));
//	color.xyz = fpos;
	frag_color = color;
}
