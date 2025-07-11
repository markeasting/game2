#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D framebuffer;

void main() {

	vec4 color = texture(framebuffer, uv);

	color.r = 1.0 - color.r;
	color.g = 1.0 - color.g;
	color.b = 1.0 - color.b;

	FragColor = color;

}
