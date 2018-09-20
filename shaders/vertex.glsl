#version 400 core
uniform mat4	model;
uniform mat4	projection;
uniform mat4	view;

in vec3			my_position;
in vec4			my_color;
in vec2			my_uv;
in vec3			my_normal;
in vec3			my_t;
in vec3			my_b;

out vec3		fnormal;
out vec2		uv;
out vec4		fcolor;
out vec4		fvertex;

mat3	get_tbn(void)
{
	vec3	t;
	vec3	b;
	vec3	n;

	t = normalize(vec3(model * vec4(my_t, 0.0)));
	b = normalize(vec3(model * vec4(my_b, 0.0)));
	n = normalize(vec3(model * vec4(my_normal, 0.0)));
	return (mat3(t, b, n));
}

void main() {
	fvertex = projection * inverse(view) * model * vec4(my_position, 1.0);
	gl_Position = fvertex;
	fcolor = my_color;
	uv = my_uv;
	fnormal = my_normal;
}
