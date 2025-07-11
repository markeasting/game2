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
        Ref<Shader> shader, 
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
    
    /**
     * @brief Assign (add) a texture to the material.
     * @param texture The texture to assign.
     * @param uniform The name of the uniform of the sampler in the shader.
     * @note The texture will be assigned to the *next* available slot.
     */
    void assignTexture(
        Ref<Texture> texture, 
        const std::string& uniform = "texture"
    );

    /**
     * @brief Assign (add) a texture to the material.
     * @param source The path to the texture file.
     * @param uniform The name of the uniform of the sampler in the shader.
     * @note The texture will be assigned to the *next* available slot.
     */
    void assignTexture(
        const char* source, 
        const std::string& uniform = "texture"
    );
    
    /**
     * glUseProgram()
     * glActiveTexture()
     * glBindTexture()
     */
    void bind() const;

private:
};
