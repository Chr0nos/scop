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

vec3		get_tangeant(void)
{
	vec3 tangeant;

	// transform tangeant from local to global space
	tangeant = inverse(mat3(view)) * mat3(model) * ftangeant;
	return tangeant;
}

mat3		get_tbn(vec3 normal)
{
	vec3	t;
	vec3	b;
	vec3	n;

	n = normalize(normal);
	t = normalize(get_tangeant());
	b = cross(t, n);
	return (mat3(t, normalize(b), n));
}

vec3		get_normal(void)
{
	vec3	normal;

	// transform normal from local to global space
	normal = inverse(mat3(view)) * mat3(model) * fnormal;
	if ((flags & FLAG_NMAP) == 0)
		return normal;
	// multiply normal map input with the tbn matrix
	// from the normal in gobal space
	return (get_tbn(normal) * (texture(normal_map, uv).rgb * 2.0 - 1.0));
}

float		make_brightness(void)
{
	vec3	fpos = fvertex.xyz;
	// get light to pos vector
	vec3	stl = light.position - fpos;
	// get intensity based on light intensity parameter and distance from the light
	float	brightness = light.intensity / clamp(length(stl), light.intensity, 500.0f);
	// multiply the directional light calculatioin from the point light
	// With the intensity computed before
	return (max(dot(get_normal(), normalize(stl)), 0.0) *  brightness);
}

float		make_directional_brightness(vec3 direction)
{
	// dot between dir and normal in global space == directionale light
	float	brightness = max(dot(get_normal(), direction), 0);
	return (clamp(brightness, 0.0, 1.0));
}

void		main() {
	vec4	color;

	if ((flags & FLAG_DBG_NORM) != 0)
		color.xyz = get_normal();
	if ((flags & FLAG_DBG_TAN) != 0)
		color.xyz = get_tangeant();
	if ((flags & FLAG_DBG) == 0)
	{
		color = texture(diffuse, uv);
		if ((flags & FLAG_NOLIGHT) == 0)
		{
			color *= light.color *
				//make_directional_brightness(AXIS_Z);
				make_brightness(); // * ((texture(ambiant_occlusion, uv) + 2.0) / 3.0);
		}
		if (tex_switch > 0)
			color = mix(color, fcolor, clamp(tex_switch, 0, 1));
			//color.xyz = fvertex.xyz;
	}
	frag_color = color;
}
