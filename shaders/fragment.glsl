#version 400 core
#define FLAG_NMAP		1u
#define FLAG_NOLIGHT	2u
#define FLAG_DBG_TAN	4u
#define FLAG_DBG_NORM	8u
#define FLAG_DBG		(FLAG_DBG_TAN | FLAG_DBG_NORM)
#define AXIS_X			vec3(1.0, 0.0, 0.0)
#define AXIS_Y			vec3(0.0, 1.0, 0.0)
#define AXIS_Z			vec3(0.0, 0.0, 1.0)
uniform sampler2D	diffuse;
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

mat3	get_tbn(void)
{
	vec3	t;
	vec3	b;
	vec3	n;

	n = normalize(fnormal);
	t = normalize(ftangeant);
	b = cross(t, n);
	return (mat3(normalize(t), normalize(b), n));
}

vec3	get_normal(void)
{
	vec3	normal;

	if ((flags & FLAG_NMAP) == 0)
		return ((vec4(fnormal, 0) * inverse(model)).xyz);
	normal = texture(normal_map, uv).rgb * 2.0 - 1.0;
	return ((vec4(normal * get_tbn(), 0) * inverse(model)).xyz);
}

float	make_brightness(void)
{
	vec3	normal = get_normal();
	vec3	fpos = vec3(transpose(view) * vec4(fvertex.xyz, 1));
	vec3	stl = light.position - fpos;
	float	brightness = dot(normal, normalize(stl));

	return (clamp(brightness, 0.2, 1.0));
}

float	make_directional_brightness(vec3 direction)
{
	float	brightness = max(dot(
				(vec4(fnormal, 0) * inverse(model)).xyz, direction), 0);

	return (clamp(brightness, 0.0, 1.0));
}

void main() {
	vec4	color;

	if ((flags & FLAG_DBG_NORM) != 0)
		color.xyz = get_normal();
	if ((flags & FLAG_DBG_TAN) != 0)
		color.xyz = (vec4(ftangeant, 0) * inverse(model)).xyz;
	if ((flags & FLAG_DBG) == 0)
	{
		color = texture(diffuse, uv);
		if ((flags & FLAG_NOLIGHT) == 0)
		{
			color *= light.color *
			//	make_directional_brightness(AXIS_X);
				make_brightness() * ((texture(ambiant_occlusion, uv) + 2.0) / 3.0);
		}
		if (tex_switch > 0)
			color = mix(color, fcolor, clamp(tex_switch, 0, 1));
	}
	frag_color = color;
}
