//
// Created by Garrett on 10/27/2021.
//
#include <graphics/vertex_array.hpp>
#include <utility>

#include <glad/gl.h>

namespace mb2dc {

gl_vertex_array::gl_vertex_array()
{
    glGenVertexArrays(1, &this->gl_id_);
}

gl_vertex_array::~gl_vertex_array()
{
    glDeleteVertexArrays(1, &this->gl_id_);
}

void gl_vertex_array::bind() const
{
    glBindVertexArray(this->gl_id_);
}

void gl_vertex_array::unbind() const
{
    glBindVertexArray(0);
}

void gl_vertex_array::set_vertex_buffer(ref<gl_vertex_buffer> buff)
{
    glBindVertexArray(this->gl_id_);

    buff->bind();

    // calculate stride from number of non-empty vertex attribute arrays
    // position + color + normal + uv
    size_t stride = (3 + 3 + 2) * sizeof(float);

    // position attribute
    size_t offset = 0;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);
    offset += 2 * sizeof(float);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);
    offset += 4 * sizeof(float);

    // uv attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);

    this->vb_ = buff;
}

void gl_vertex_array::set_index_buffer(ref<gl_index_buffer> buff)
{
    glBindVertexArray(this->gl_id_);

    this->ib_ = std::move(buff);
    this->ib_->bind();
}

}
