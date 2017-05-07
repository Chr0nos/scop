#version 400
uniform mat4 model;
uniform mat4 projection;

layout (location = 0) in vec3 vp;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

void main() {
	gl_Position = projection * model * vec4(vp, 1.0);
	uv = vertexUV;
}
