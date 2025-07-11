#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D framebuffer;

void main() {

	vec4 color = texture(framebuffer, uv);

	float smearAlpha = 0.05;

	FragColor = vec4(color.rgb, smearAlpha);

}
