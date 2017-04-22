#version 400
uniform mat4 model;
uniform mat4 projection;
layout (location = 0) in vec3 vp;
void main() {
	gl_Position = (model * projection) * vec4(vp, 1.0);
}
