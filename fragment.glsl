#version 400 core
uniform sampler2D	texture_sampler;
uniform sampler2D	normal_map;
uniform float		tex_switch;
uniform int			tex_mode;
uniform struct		s_light {
	vec3			position;
	vec4			color;
}					light;
uniform mat4		model;

in vec2				uv;
in vec4				fcolor;
in vec3				fnormal;
in vec4				fvertex;

out vec4			frag_color;

float	make_brightness(void)
{
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normal_matrix * fnormal);
//	vec3 normal = normalize(fnormal);
//	vec3 normal = normalize(texture(normal_map, uv).xyz);
//	vec3 normal = normalize(normal_matrix * texture(normal_map, uv).xyz);
	vec3 fpos = vec3(model * vec4(fvertex.xyz, 1));
	//a vector pointing to the light
	vec3 stl = light.position * mat3(model) - fpos;
	float brightness = dot(normal, stl) / (length(stl) * length(normal));
	brightness = clamp(brightness, 0.2, 1.0);
//	return (1);
	return (brightness);
}

void main() {
	vec4 color = texture(texture_sampler, uv);
	float brightness = make_brightness();
	color *= light.color * brightness;
	color = mix(color, fcolor, clamp(tex_switch, 0, 1));
	frag_color = color;
}
