//
// Created by Garrett on 10/27/2021.
//

#include <graphics/buffer.hpp>

#include <glad/gl.h>

namespace mb2dc {

static uint32_t get_usage_type(buffer_usage_type usage)
{
    switch (usage) {
        case STATIC_DRAW:
            return GL_STATIC_DRAW;
        case DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        case STREAM_DRAW:
            return GL_STREAM_DRAW;
        case STATIC_READ:
            return GL_STATIC_READ;
        case DYNAMIC_READ:
            return GL_DYNAMIC_READ;
        case STREAM_READ:
            return GL_STREAM_READ;
        case STATIC_COPY:
            return GL_STATIC_COPY;
        case DYNAMIC_COPY:
            return GL_DYNAMIC_COPY;
        case STREAM_COPY:
            return GL_STREAM_COPY;
    }

    return INT_MAX;
}

gl_vertex_buffer_t::gl_vertex_buffer_t(const std::vector<vertex_t> &vertices, buffer_usage_type usage)
    : gl_vertex_buffer_t(vertex_t::to_floats(vertices), {2, 4, 2}, usage)
{
}

gl_vertex_buffer_t::gl_vertex_buffer_t(const std::vector<float> &vertices, const std::vector<uint8_t> &attributes,
                                       buffer_usage_type usage)
        : usage_(usage), attributes_(attributes), vertices_(vertices)
{
    glGenBuffers(1, &this->gl_id_);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            reinterpret_cast<const void *>(vertices.data()),
            get_usage_type(usage)
    );
}

gl_vertex_buffer_t::~gl_vertex_buffer_t()
{
    glDeleteBuffers(1, &this->gl_id_);
}

void gl_vertex_buffer_t::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
}

void gl_vertex_buffer_t::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int gl_vertex_buffer_t::get_data_size() const
{
    unsigned int size = 0;

    for (auto &a: this->attributes_) {
        size += a;
    }

    return this->vertices_.size() / size;
}

void gl_vertex_buffer_t::write_data(void *vertices, uint64_t size, uint64_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id_);
    glBufferSubData(
            GL_ARRAY_BUFFER,
            offset,
            size * sizeof(float),
            reinterpret_cast<const void *>(vertices)
    );

    this->vertices_.clear();
    this->vertices_.reserve(size);

    auto v = static_cast<float *>(vertices);

    for (auto i = 0; i < size; i++) {
        this->vertices_.push_back(v[i]);
    }
}

gl_index_buffer_t::gl_index_buffer_t(const std::vector<uint32_t> &indices, buffer_usage_type usage)
        : usage_(usage)
{
    glGenBuffers(1, &this->gl_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id_);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices.data()),
            get_usage_type(usage)
    );

    this->count_ = indices.size();
    this->indices_ = indices;
}

gl_index_buffer_t::~gl_index_buffer_t()
{
    glDeleteBuffers(1, &this->gl_id_);
}

void gl_index_buffer_t::bind()
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
    glBufferSubData(
            GL_ELEMENT_ARRAY_BUFFER,
            offset,
            size * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices)
    );

    this->count_ = size;
}

}