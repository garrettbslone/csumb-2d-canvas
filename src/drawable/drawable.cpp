//
// Created by Garrett on 8/4/22.
//

#include <drawable/drawable.hpp>
#include <glad/gl.h>
#include <glm/ext.hpp>

namespace mb2dc {

void drawable_t::draw() const
{
    this->va_->bind();
    auto ib = this->va_->get_index_buffer();

    if (ib && ib->get_count() > 0) {
        glDrawElements(
                get_topology(this->topo_),
                (GLint) ib->get_count(),
                GL_UNSIGNED_INT,
                nullptr);
    } else {
        glDrawArrays(
                get_topology(this->topo_),
                0,
                (GLint) this->va_->get_vertex_buffers()->get_data_size());
    }
}

void drawable_t::set_z_index(float z)
{
    this->shader_->set_uniform_float("zIndex", z);
}

void drawable_t::translate(const glm::vec2 &v)
{
    this->model_ = glm::translate(this->model_, glm::vec3(v.x, v.y, 0));
}

void drawable_t::rotate(float deg, const glm::vec3 &axis)
{
    this->model_ = glm::rotate(this->model_, deg, axis);
}

void drawable_t::scale(const glm::vec2 &v)
{
    this->model_ = glm::scale(this->model_, glm::vec3{v.x, v.y, 1.f});
}

}