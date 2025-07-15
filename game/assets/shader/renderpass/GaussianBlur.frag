#version 330 core
out vec4 FragColor;
  
in vec2 uv;

uniform sampler2D u_readBuffer;

uniform bool u_horizontal;

/* https://github.com/Experience-Monks/glsl-fast-gaussian-blur */
vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.3333333333333333) * direction;
  color += texture(image, uv) * 0.29411764705882354;
  color += texture(image, uv + (off1 / resolution)) * 0.35294117647058826;
  color += texture(image, uv - (off1 / resolution)) * 0.35294117647058826;
  return color; 
}

void main()
{             
    // vec2 tex_offset = 1.0 / textureSize(u_readBuffer, 0); // gets size of single texel
    vec2 resolution = vec2(textureSize(u_readBuffer, 0));

    // vec2 resolution = vec2(768, 768); // @todo set as uniform

    FragColor = blur5(
        u_readBuffer, 
        uv, 
        resolution, 
        u_horizontal 
            ? vec2(1.0, 0.0) // Note: you can smear these by going > 1.0
            : vec2(0.0, 1.0)
    );
}
