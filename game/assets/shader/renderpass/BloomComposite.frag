#version 330 core
out vec4 FragColor;
  
in vec2 uv;

uniform sampler2D u_readBuffer;
uniform sampler2D u_bloomBuffer;

uniform float u_exposure = 1.0;
uniform float u_gamma = 1.0; // 2.2

void main()
{
    vec3 hdrColor = texture(u_readBuffer, uv).rgb;      
    vec3 bloomColor = texture(u_bloomBuffer, uv).rgb;

    /* Debugging */
    // FragColor = vec4(bloomColor, 1.0);

    /* Additive blending */
    hdrColor += bloomColor; 
    
    // /* Tone mapping (exp) */
    // vec3 result = vec3(1.0) - exp(-hdrColor * u_exposure);
    
    // /* Reinhard tone mapping */
    // // vec3 result = hdrColor / (hdrColor + vec3(1.0));

    // /* Gamma correction */
    // result = pow(result, vec3(1.0 / u_gamma));
    // FragColor = vec4(result, 1.0);

    FragColor = vec4(hdrColor, 1.0);
} 
