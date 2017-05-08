#version 400
uniform mat4 model;
uniform mat4 projection;

//layout (location = 0) in vec3 vp;
//layout(location = 1) in vec2 vertexUV;
in vec3	my_position;
in vec4 my_color;
in vec2 my_uv;
in vec3 my_normal;

out vec2 uv;
out vec4 fcolor;
out vec3 fnormal;

void main() {
	gl_Position = projection * model * vec4(my_position, 1.0);
	uv = my_uv;
	fcolor = my_color;
	fnormal = my_normal;
}
