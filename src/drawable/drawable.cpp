//
// Created by Garrett on 8/4/22.
//

#include <drawable/drawable.hpp>
#include <core/canvas.hpp>

#include <glad/gl.h>

namespace mb2dc {

drawable_t::~drawable_t()
{
    auto _canvas = canvas_t::get();

    if (_canvas) {
        _canvas->queue_.dequeue(this);
    }
}

void drawable_t::draw() const
{
    this->draw(glm::mat4(1.f));
}

void drawable_t::draw(const glm::mat4 &view_proj) const
{
    this->va_->bind();
    this->shader_->bind();
    this->shader_->set_uniform_float("zIndex", this->z_index_);

    auto num_textures = this->textures_.size();

    if (num_textures == 1) {
        this->shader_->enable_texture("uTex", this->textures_.begin()->second);
    } else if (num_textures > 1) {
        // for now default to "uTex<texture.slot()>" for uniform names.
        // a better way to this would probably be to use the name as the uniform name
        // but some enforcement of names would be required.
        for (auto &[name, texture]: this->textures_) {
            this->shader_->enable_texture(std::string("uTex") + std::to_string(texture->slot()), texture);
        }
    }

    this->shader_->set_uniform_mat4("MVP", this->model_ * view_proj);

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

void drawable_t::freeze()
{
    this->frozen_ = true;
}

void drawable_t::unfreeze()
{
    this->frozen_ = false;
}

void drawable_t::translate(const glm::vec2 &v, bool reset)
{
    if (this->frozen_) {
        return;
    }

    if (reset) {
        this->center_ = v;
    } else {
        this->center_ += v;
    }

    this->make_model_mat();
}

void drawable_t::rotate(float deg, bool reset)
{
    if (this->frozen_) {
        return;
    }

    deg = glm::radians(deg);

    if (reset) {
        this->rotation_ = deg;
    } else {
        this->rotation_ += deg;
    }

    this->make_model_mat();
}

void drawable_t::scale(const glm::vec2 &v, bool reset)
{
    if (this->frozen_) {
        return;
    }

    if (reset) {
        this->scale_ = v;
    } else {
        this->scale_ += v;
    }

    this->make_model_mat();
}

void drawable_t::set_z_index(float z)
{
    if (this->frozen_) {
        return;
    }

    auto _canvas = canvas_t::get();

    if (_canvas) {
        _canvas->queue_.adjust(this, z);
    }
}

bool drawable_t::is_slot_active(uint8_t slot)
{
    // there are only 48 possible texture slots [0, 47]
    if (slot >= MAX_TEXTURE_SLOTS) {
        return false;
    }

    return ((this->active_slots_ >> slot) & 1) == 1;
}

bool drawable_t::set_slot_active(uint8_t slot)
{
    // slot is already in use
    if (((this->active_slots_ >> slot) & 1) == 1) {
        return false;
    }

    this->active_slots_ |= 1 << slot;
    return true;
}

uint8_t drawable_t::find_next_slot()
{
    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        if (!is_slot_active(i)) {
            set_slot_active(i);
            return i;
        }
    }

    return -1;
}

void drawable_t::set_state(uint16_t state, bool clear_previous)
{
    if (clear_previous) {
        this->state_ = 0;
    }

    this->state_ |= state;
}

void drawable_t::make_model_mat()
{
    const float cos_rot = glm::cos(this->rotation_);
    const float sin_rot = glm::sin(this->rotation_);

    this->model_ = glm::mat4{
            {this->scale_.x * cos_rot,         this->scale_.x * sin_rot,         0.f, 0.f},
            {this->scale_.y * -sin_rot,        this->scale_.y * cos_rot,         0.f, 0.f},
            {0.f,                              0.f,                              1.f, 0.f},
            {this->center_.x * this->scale_.x, this->center_.y * this->scale_.y, 0.f, 1.f}};
}

}