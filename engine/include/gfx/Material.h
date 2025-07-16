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

    ~Material() {
        // std::cout << "[Material] Destroying material" << std::endl;
    };

    /**
     * @brief Set a uniform value.
     * @param name The name of the uniform in the shader.
     * @param value The value to set.
     * @tparam T The type of the uniform value.
     * @note The uniform is saved, but only applied during the next bind() call 
     * (e.g. when a new frame is drawn)
     */
    template <typename T>
    void setUniform(
        const std::string& name, 
        T value
    ) {

        try {
            auto uni = getUniform<T>(name);
            uni->set(value);
        } catch (const std::runtime_error& e) {
            assignUniform(name, ref<Uniform<T>>(value));
        }
    }

    /**
     * @brief Get a uniform by name.
     * @param name The name of the uniform.
     * @param createIfNotFound If true, creates a new uniform. Throws a runtime error otherwise. 
     */
    template <typename T>
    Ref<Uniform<T>> getUniform(
        const std::string& name, 
        bool createIfNotFound = false
    ) {
        if (uniforms.find(name) != uniforms.end()) {
            return std::static_pointer_cast<Uniform<T>>(uniforms.at(name));
        } else {
            if (createIfNotFound) {
                printf("[Material] Uniform not found: %s, creating new.\n", name.c_str());
                auto uniform = ref<Uniform<T>>();
                assignUniform(name, uniform);
                return uniform;
            } else {
                throw std::runtime_error("[Material] Uniform not found: " + name);
            }
        }
    }

    /**
     * @brief Assign (add) a uniform to the material.
     * @param name The name of the uniform in the shader.
     * @param uniform The uniform to assign.
     */
    void assignUniform(
        const std::string name, 
        Ref<IUniform> uniform
    );
    
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
     * @brief Assign (add) a texture to the material.
     * @param texture The OpenGL texture ID.
     * @param textureUnit Texture unit, e.g. GL_TEXTURE0
     * @param uniform The name of the uniform of the sampler in the shader.
     */
    void setTexture(
        const std::string& uniform,
        GLenum textureUnit,
        GLuint texture
    );

    /**
     * @brief Bind the material to the current OpenGL context.
     * @param bindTextures If true, binds all textures to their respective texture units.
     * @see glUseProgram() glActiveTexture() glBindTexture()
     */
    void bind(bool bindTextures = true) const;

private:
};
