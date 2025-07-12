#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D u_readBuffer;

void main() {

	vec4 color = texture(u_readBuffer, uv);

	FragColor = color;

}
