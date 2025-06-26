#pragma once

#include "gfx/Texture.h"

#include <array>
#include <string>

class CubeMapTexture : public Texture {
public: 

    CubeMapTexture() = default;
    CubeMapTexture(const std::array<std::string, 6>& faces);

    /**
     * Binds the cubemap texture using GL_TEXTURE_CUBE_MAP
     */
    void bind() const;

    /**
     * Loads the cubemap texture from 6 file paths: 
     * - GL_TEXTURE_CUBE_MAP_POSITIVE_X (right)
     * - GL_TEXTURE_CUBE_MAP_NEGATIVE_X (left)
     * - GL_TEXTURE_CUBE_MAP_POSITIVE_Y (bottom)
     * - GL_TEXTURE_CUBE_MAP_NEGATIVE_Y (top)
     * - GL_TEXTURE_CUBE_MAP_POSITIVE_Z (front)
     * - GL_TEXTURE_CUBE_MAP_NEGATIVE_Z (back)
     */
    void loadCubemap(const std::array<std::string, 6>& faces);
};