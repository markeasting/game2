#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D u_readBuffer;
uniform float u_smearAlpha = 0.05;
uniform float u_smearStart = 0.0; 	// Start of smear effect (vignette)
uniform float u_smearEnd = 0.5; 	// End of smear effect (vignette)

bool useVignette = true; 			// Toggle for vignette effect
// bool useBrightnessThreshold = true; // Toggle for brightness threshold

void main() {

	vec4 color = texture(u_readBuffer, uv);

	float smear = u_smearAlpha;

	if (useVignette) {
		// Distance from the center of the screen
		float dist = length(uv - vec2(0.5, 0.5));

		// Vignette effect to darken the edges of the screen
		float vignette = 1.0 - smoothstep(u_smearStart, u_smearEnd, dist);

		// Debug: show vignette effect
		// color.rgb *= vignette;
		
		// Use the vignette to control the alpha of the smear effect
		smear = mix(u_smearAlpha, 1.0, vignette);
	}

	// if (useBrightnessThreshold) {
	// 	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
		
	// 	if (brightness < 1.1) {
	// 		smear = 1.0;
	// 	}
	// }

	FragColor = vec4(color.rgb, smear);

}
