#version 400
out vec4 frag_colour;
void main() {
	vec4 lightcolor = vec4(1.0);
	frag_colour = vec4(0.5, 0.2, 0.5, 1.0) * lightcolor;
}
