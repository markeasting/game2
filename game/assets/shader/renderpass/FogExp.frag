#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 uv;

uniform sampler2D u_readBuffer;
uniform sampler2D u_depth;

uniform float u_fogDensity = 0.1; // Density of the fog
uniform vec4 u_fogColor = vec4(1.0, 1.0, 1.0, 0.5);
uniform float u_fogStart = 5.0;
uniform float u_fogEnd = 100.0;

float linearize_depth(float d, float zNear, float zFar)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}

void main() {

	vec4 inputColor = texture(u_readBuffer, uv);
    
    // Depth in [0, 1]
    vec4 depthRaw = texture(u_depth, uv); // First channel (r) contains depth in [0, 1]

    // Depth in meters
    float depth = linearize_depth(depthRaw.r, 0.01, 500.0);
    
    // Exponential fog calculation
    float fogFactor = 1.0 - exp(-u_fogDensity * depth);

    // Alpha blending with fog color
    FragColor = vec4(
        mix(inputColor.rgb, u_fogColor.rgb, fogFactor * u_fogColor.a),
        1.0
    );

    // Debug
    // FragColor = vec4(vec3(depth), 1.0);
}
