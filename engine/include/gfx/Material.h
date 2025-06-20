#pragma once

#include "gfx/Shader.h"
#include "gfx/Uniforms.h"
#include "gfx/Texture.h"

#include <stdexcept>
#include <vector>

class Material {
public:

    Ref<Shader> m_shader = nullptr;
    
    std::unordered_map<std::string, Ref<IUniform>> uniforms = {};

    std::vector<Ref<Texture>> textures = {};

    bool wireframe = false;
    bool transparent = false;

    Material(
        const std::string& shaderBasePath, 
        std::unordered_map<std::string, Ref<IUniform>> uniforms = {}
    );
    
    Material(
        const std::string& vert, 
        const std::string& frag, 
        std::unordered_map<std::string, Ref<IUniform>> uniforms = {}
    );

    ~Material() = default;

    template <typename T>
    void setUniform(const std::string& name, T value) {

        try {
            auto uni = getUniform<T>(name);
            uni->set(value);
        } catch (const std::runtime_error& e) {
            assignUniform(name, ref<Uniform<T>>(value));
        }
    }

    template <typename T>
    Ref<Uniform<T>> getUniform(const std::string& name) {

        if (uniforms.find(name) != uniforms.end()) {
            return std::static_pointer_cast<Uniform<T>>(uniforms.at(name));
        } else {
            throw std::runtime_error("[Material] Uniform not found: " + name);
        }
    }

    void assignUniform(const std::string name, Ref<IUniform> uniform);
    
    void assignTexture(
        Ref<Texture> texture, 
        const std::string& uniform = "texture"
    );

    void assignTexture(
        const char* source, 
        const std::string& uniform = "texture"
    );
    
    void bind() const;

private:
};
