#version 400 core
uniform sampler2D	texture_sampler;
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

void main() {
	vec4 color = texture(texture_sampler, uv);
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normal_matrix * fnormal);
	vec3 fpos = vec3(model * vec4(fvertex.xyz, 1));
	//a vector pointing to the light
	vec3 stl = light.position - fpos;
	float brightness = dot(normal, stl) / (length(stl) * length(normal));

	brightness = clamp(brightness, 0.2, 1.0);
	color = mix(color, fcolor, clamp(tex_switch, 0, 1));
	color *= light.color;
	frag_color = color;
}
