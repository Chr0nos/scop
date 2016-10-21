#version 400
in vec3 vp;
in vec3 color;

void	main() {
	gl_Position = vec4(vp, 1.0);
	gl_Color = color;
}
