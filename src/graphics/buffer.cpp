//
// Created by Garrett on 10/27/2021.
//

#include <graphics/buffer.hpp>

#include <glad/gl.h>

namespace mb2dc {

gl_vertex_buffer_t::gl_vertex_buffer_t(const std::vector<vertex_t> &vertices)
{

    auto data = vertex_t::to_floats(vertices);

    glGenBuffers(1, &this->gl_id_);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ARRAY_BUFFER,
            data.size() * sizeof(float),
            reinterpret_cast<void *>(data.data()),
            GL_STATIC_DRAW
    );

    this->vertices_ = vertices;
}

gl_vertex_buffer_t::~gl_vertex_buffer_t()
{
    glDeleteBuffers(GL_ARRAY_BUFFER, &this->gl_id_);
}

void gl_vertex_buffer_t::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
}

void gl_vertex_buffer_t::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl_vertex_buffer_t::write_data(void *vertices, uint64_t size, uint64_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ARRAY_BUFFER,
            size * sizeof(float),
            reinterpret_cast<const void *>(vertices),
            GL_STATIC_DRAW
    );

    this->vertices_.clear();
    this->vertices_.reserve(size);

    auto v = static_cast<vertex_t *>(vertices);

    for (auto i = 0; i < size; i++) {
        this->vertices_.push_back(v[i]);
    }
}

gl_index_buffer_t::gl_index_buffer_t(const std::vector<uint32_t> &indices)
{
    glGenBuffers(1, &this->gl_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices.data()),
            GL_STATIC_DRAW);

    this->count_ = indices.size();
    this->indices_ = indices;
}

gl_index_buffer_t::~gl_index_buffer_t()
{
    glDeleteBuffers(1, &this->gl_id_);
}

void gl_index_buffer_t::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id_);
}

void gl_index_buffer_t::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void gl_index_buffer_t::write_data(void *indices, uint64_t size, uint64_t offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            size * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices),
            GL_STATIC_DRAW
    );

    this->count_ = size;
}

}