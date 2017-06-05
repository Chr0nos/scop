#version 400 core
uniform mat4	model;
uniform mat4	projection;
uniform mat4	view;

in vec3			my_position;
in vec4			my_color;
in vec2			my_uv;
in vec3			my_normal;

out vec3		fnormal;
out vec2		uv;
out vec4		fcolor;
out vec4		fvertex;

mat3	get_tbn(void)
{
	vec3	t;
	vec3	b;
	vec3	n;
	mat3	tbn;

	n = normalize(my_normal);
	tbn = mat3(t, b, n);
	return (tbn);
}

void main() {
	fvertex = projection * inverse(view) * model * vec4(my_position, 1.0);
	gl_Position = fvertex;
	fcolor = my_color;
	uv = my_uv;
	fnormal = my_normal;
}
