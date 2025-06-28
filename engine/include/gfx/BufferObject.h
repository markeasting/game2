#pragma once

#include "common/gl.h"
#include "gfx/Vertex.h"

#include <vector>

class IBufferObject {
public:

    IBufferObject() = default;

    ~IBufferObject() {
        glDeleteBuffers(1, &m_buffer);
    }

    // void setSize(unsigned int size) {
    //     m_size = size;
    // }

    /* Binding will be performed by VAO */
    // virtual void bind() const {};

    inline unsigned int getCount() const {
        return m_count;
    }

    inline unsigned int getSize() const {
        return m_size;
    }

    // virtual void bind();

protected:

	GLuint m_buffer = 0;
    GLenum m_usage = GL_STATIC_DRAW;
	
    // @todo maybe store individual element size as well

    /**
     * Number of elements in the buffer.
     */
	unsigned int m_count = 0;

    /**
     * Total buffer size in bytes.
     * Usually equals `m_count * sizeof(T)`
     */
	unsigned int m_size = 0;
};

template <typename T = float>
class BufferObject : public IBufferObject {
public:

    std::vector<T> m_data = {};

    BufferObject(const std::vector<T>& data = {}) : IBufferObject() {
        GLint vao;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        assert(vao != 0 && "BufferObject - A VAO must be bound before creating a BufferObject.");

        m_data = data;
        m_count = data.size();
        m_size = m_count * sizeof(T);
        // this->set(data); // Must be called from Derived class...?

        glGenBuffers(1, &m_buffer);
    }

    /* Write data to an OpenGL Buffer Object. Assumes a VAO is bound when invoked. */
    virtual void set(const std::vector<T>& data) {}
};

class IndexBuffer : public BufferObject<unsigned int> {
public:

    IndexBuffer(const std::vector<unsigned int>& data = {}) : BufferObject(data) {
        this->set(data);
    }

    void set(const std::vector<unsigned int>& data) override { 
        // GLint vao;
        // glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        // assert(vao != 0 && "IndexBuffer - A VAO must be bound before using IndexBuffer.");

        // if (m_buffer == 0) 
        //     glGenBuffers(1, &m_buffer);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, &m_data[0], m_usage);
        // glNamedBufferData(m_buffer, m_size, &m_data[0], m_usage); // DSA
    }

    // void bind() override {
    //     // glBindBuffer(GL_ARRAY_BUFFER, m_buffer); // not used when using a VAO
    // }
};

class VertexBuffer : public BufferObject<Vertex> {
public:

    VertexBuffer(const std::vector<Vertex>& data = {}) : BufferObject(data) {
        this->set(data);
    }

    void set(const std::vector<Vertex>& data) override {
        // GLint vao;
        // glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        // assert(vao != 0 && "VertexBuffer - A VAO must be bound before using VertexBuffer.");

        // if (m_buffer == 0) 
        //     glGenBuffers(1, &m_buffer);

        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ARRAY_BUFFER, m_size, &m_data[0], m_usage);
        // glNamedBufferData(m_buffer, m_size, &m_data[0], m_usage); // DSA
    }

    // void bind() override {
    //     // glBindBuffer(GL_ARRAY_BUFFER, m_buffer); // not used when using a VAO
    // }
};
