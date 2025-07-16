#version 330 core
// https://github.com/paulpela/lighting-opengl/tree/master/shaders

in VS_OUT {
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

uniform samplerCube u_cubemap;

uniform vec3 ambient = vec3(0.15, 0.0, 0.0);
uniform vec3 diffuseAlbedo = vec3(0.0, 0.0, 0.0);
uniform vec3 specularAlbedo = vec3(1.0, 0.8, 0.8);
uniform vec3 rimColor = vec3(0.3, 0.3, 0.3) * 2.0;

uniform float shininess = 100.0;
uniform float rimPower = 3;

uniform int specularOn = 1;
uniform int rimLightOn = 1;

out vec4 out_color;

vec3 rimLight(vec3 N, vec3 V, vec3 color) {

    float f = 1.0 - dot(N, V);

    f = smoothstep(0.0, 1.0, f);

    f = pow(f, rimPower);

    return f * color;
}

void main() {
    // vec3 N = normalize(fs_in.N);
    // vec3 L = normalize(fs_in.L);
    // vec3 V = normalize(fs_in.V);
    vec3 N = fs_in.N;
    vec3 L = fs_in.L;
    vec3 V = fs_in.V;

    vec3 R = reflect(-L, N);

    vec3 diffuse = max(dot(N, L), 0.0) * diffuseAlbedo;

    vec3 specular = vec3(0.0);

    if (specularOn == 1) {
        specular = pow(max(dot(R, V), 0.0), shininess) * specularAlbedo;
    }

    // Cubemap for environment mapping
    vec3 reflectedDir = reflect(-V, N);
    vec3 envColor = texture(u_cubemap, -reflectedDir).rgb;
    // out_color = vec4(envColor.rgb, 1.0);

    if (rimLightOn == 1) {
        vec3 rimming = rimColor + envColor;
        out_color = vec4(ambient + diffuse + specular + rimLight(N, V, rimming), 1.0);
    } else {
        out_color = vec4(ambient + diffuse + specular, 1.0);
    }

    out_color.rgb += (envColor * specularAlbedo) * 0.5;
}
