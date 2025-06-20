
#include "gfx/Material.h"

#include <string>
#include <cassert>

Material::Material(
    const std::string& shaderBasePath, 
    std::unordered_map<std::string, Ref<IUniform>> uniforms
) {
    m_shader = ref<Shader>(shaderBasePath);

    assert(m_shader != nullptr);
    // assert(m_shader->m_program != 0);

    for (const auto &[key, uniform] : uniforms) {
        assignUniform(key, uniform);
    }
}

Material::Material(
    const std::string& vert, 
    const std::string& frag, 
    std::unordered_map<std::string, Ref<IUniform>> uniforms
) {
    m_shader = ref<Shader>(vert, frag);

    assert(m_shader != nullptr);
    // assert(m_shader->m_program != 0);

    for (const auto &[key, uniform] : uniforms) {
        assignUniform(key, uniform);
    }
}

void Material::assignUniform(
    const std::string name, 
    Ref<IUniform> uniform
) {

    assert(uniform != nullptr);

    if (uniforms[name] != nullptr) {
        throw std::runtime_error("[Material] Uniform already exists: " + name);
        return;
    }

    uniform->setLocation(m_shader->getUniformLocation(name));
    uniforms[name] = uniform;
}

void Material::assignTexture(
    Ref<Texture> texture, 
    const std::string& uniform
) {
    this->setUniform(uniform, (int) textures.size());

    /** @TODO this doesn't seem to work, most textures have alpha data but aren't actually transparent. */
    /** Maybe just set `transparent` manually. */
    // if (texture->m_format == GL_RGBA)
    //     transparent = true;

    textures.push_back(texture);
}

void Material::assignTexture(
    const char* source, 
    const std::string& uniform
) {

    auto texture = ref<Texture>(source);

    this->assignTexture(texture, uniform);
}

void Material::bind() const {

    m_shader->bind();
    // glUseProgram(m_shader->m_program);

    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // @todo is this needed? Move to Texture::bind()?
        // glBindTexture(GL_TEXTURE_2D, textures[i]->m_texture);
        textures[i]->bind();
    }

    for (const auto &[key, uniform] : uniforms) {
        uniform->bind();
    }
}
