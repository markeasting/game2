#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 uv;

uniform sampler2D u_readBuffer;
uniform float u_bloomThreshold = 0.8;

/** 
 * https://learnopengl.com/Advanced-Lighting/Bloom
 */
void main() {

	vec4 color = texture(u_readBuffer, uv);

	FragColor = color;
    // BrightColor = color;

	// Check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > u_bloomThreshold)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}
