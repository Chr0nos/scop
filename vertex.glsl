#version 400 core
uniform mat4 model;
uniform mat4 projection;

in vec3	my_position;
in vec4 my_color;
in vec2 my_uv;
in vec3 my_normal;

out vec3 fnormal;
out vec2 uv;
out vec4 fcolor;

void main() {
	gl_Position = projection * model * vec4(my_position, 1.0);
	fcolor = my_color;
	uv = my_uv;
	fnormal = my_normal;
}
