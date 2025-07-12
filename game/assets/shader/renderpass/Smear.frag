#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D u_readBuffer;
uniform float u_smearAlpha = 0.05;

void main() {

	vec4 color = texture(u_readBuffer, uv);

	FragColor = vec4(color.rgb, u_smearAlpha);

}
