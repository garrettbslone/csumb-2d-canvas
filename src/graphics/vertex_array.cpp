//
// Created by Garrett on 10/27/2021.
//
#include <graphics/vertex_array.hpp>

#include <glad/gl.h>

#include <utility>

namespace mb2dc {

gl_vertex_array_t::gl_vertex_array_t()
{
    glGenVertexArrays(1, &this->gl_id_);
}

gl_vertex_array_t::~gl_vertex_array_t()
{
    glDeleteVertexArrays(1, &this->gl_id_);
}

void gl_vertex_array_t::bind()
{
    glBindVertexArray(this->gl_id_);
}

void gl_vertex_array_t::unbind() const
{
    glBindVertexArray(0);
}

void gl_vertex_array_t::set_vertex_buffer(ref_t<gl_vertex_buffer_t> buff)
{
    glBindVertexArray(this->gl_id_);

    buff->bind();

    int stride = 0, curr = 0;
    size_t offset = 0;

    for (auto &a: buff->attributes_) {
        stride += a;
    }

    stride *= sizeof(float);

    for (auto &a: buff->attributes_) {
        glEnableVertexAttribArray(curr);
        glVertexAttribPointer(curr, a, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);

        offset += a * sizeof(float);
        curr++;
    }

    this->vb_ = buff;
}

void gl_vertex_array_t::set_index_buffer(ref_t<gl_index_buffer_t> buff)
{
    glBindVertexArray(this->gl_id_);

    this->ib_ = std::move(buff);
    this->ib_->bind();
}

}
