#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D u_readBuffer;
uniform float u_smearAlpha = 0.05;
uniform float u_smearStart = 0.0; // Start of smear effect
uniform float u_smearEnd = 0.5; // End of smear effect

void main() {

	vec4 color = texture(u_readBuffer, uv);

	// Distance from the center of the screen
	float dist = length(uv - vec2(0.5, 0.5));

	// Vignette effect to darken the edges of the screen
	float vignette = 1.0 - smoothstep(u_smearStart, u_smearEnd, dist);

	// Apply the vignette effect to the color
	// color.rgb *= vignette;
	
	// Use the vignette to control the alpha of the smear effect
	float smear = mix(u_smearAlpha, 1.0, vignette);

	FragColor = vec4(color.rgb, smear);

}
