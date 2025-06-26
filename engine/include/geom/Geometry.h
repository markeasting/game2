#pragma once

#include "objload.h"

#include "common/ref.h"
#include "common/glm.h"

#include "gfx/BufferObject.h"

#include <vector>

class Renderer;

class Geometry {
public:

    Geometry();
    Geometry(Ref<VertexBuffer> vbo, Ref<IndexBuffer> ibo = nullptr);
    Geometry(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices = {});
    Geometry(const obj::Model &objModel);

    // template<typename T>
    // void setAttribute(BufferObject<T> attribute) {
    //     auto buffer = std::dynamic_pointer_cast<BufferObject<T>>(m_attributes.at(name));
    //     buffer->set(value);
    // };

    void setData(Ref<VertexBuffer> vbo, Ref<IndexBuffer> ibo = nullptr);

    void bind() const;
    void unbind() const;

    bool hasIndices() const { return m_indexBuffer != nullptr; }

// protected:
    /* @TODO make these unique ptr? This class should 'own' the buffers */
    Ref<IndexBuffer> m_indexBuffer = nullptr;
    Ref<VertexBuffer> m_vertexBuffer = nullptr;
    // Ref<BufferObject<float>> m_vertexBuffer = nullptr;

protected:
    friend Renderer;

    GLuint m_vao = 0;
    // bool m_attribPointerSet = false;

};
