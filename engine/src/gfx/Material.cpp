
#include "gfx/Material.h"
#include "common/gl.h"
#include "gfx/Shader.h"

#include <string>
#include <cassert>

Material::Material(
    Ref<Shader> shader, 
    std::unordered_map<std::string, Ref<IUniform>> uniforms
) {
    m_shader = shader;

    assert(m_shader != nullptr);
    assert(m_shader->m_program != 0);

    for (const auto &[key, uniform] : uniforms) {
        assignUniform(key, uniform);
    }
}

void Material::assignUniform(
    const std::string name, 
    Ref<IUniform> uniform
) {

    assert(uniform != nullptr);

    if (uniforms.find(name) != uniforms.end()) {
        throw std::runtime_error("[Material] Uniform already exists: " + name);
        return;
    }

    GLint location = m_shader->getUniformLocation(name);
    
    // if (location == -1) {
    //     throw std::runtime_error("[Material] Uniform location not found: " + name);
    // }

    uniform->setLocation(location);
    uniforms[name] = uniform; //uniforms.try_emplace(name, uniform);
}

void Material::assignTexture(
    Ref<Texture> texture, 
    const std::string& uniform
) {
    this->setUniform(uniform, (int) textures.size());

    /* @TODO this doesn't work, most textures have alpha but aren't *actually* transparent. */
    /** Maybe just set `transparent` manually. */
    // if (texture->m_format == GL_RGBA)
    //     transparent = true;

    textures.push_back(texture);
}

void Material::assignTexture(
    const char* source, 
    const std::string& uniform
) {
    auto texture = ref<Texture>();
    texture->load(source);

    this->assignTexture(texture, uniform);
}

void Material::setTexture(
    const std::string& uniform,
    GLenum textureUnit,
    GLuint texture
) {
    const auto& uni = getUniform<int>(uniform, true);

    uni->set((int) (textureUnit - GL_TEXTURE0));
    uni->bind(); // Also uploads the value to the shader.

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Material::bind(bool bindTextures) const {

    m_shader->bind(); // glUseProgram()

    if (bindTextures) {
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // @todo move to bind()?
            textures[i]->bind();
        }
    }

    for (const auto &[key, uniform] : uniforms) {
        uniform->bind();
    }
}
