//
// Created by Garrett on 8/4/22.
//

#include <core/canvas.hpp>
#include <core/exception.hpp>
#include <graphics/shader.hpp>

#include<glm/ext.hpp>

namespace mb2dc {

canvas *canvas::instance_ = nullptr;

canvas::canvas(const window_spec &spec)
{
    if (instance_)
        throw canvas_ex("app already exists!");

    instance_ = this;

    this->window_ = new_ref<window>(spec, window_data {
            nullptr, nullptr, nullptr
    });
    this->running_ = true;
    this->minimized_ = false;

    this->gl_ctx_ = new_ref<gl_context>(this->window_->get_native_window());

//    double w = this->spec_.width_ / 2, h  = this->spec_.height_ / 2;
//    this->proj_ = glm::ortho(-w, w, -h, h, 0.1, 1000.);
    this->proj_ = glm::mat4(1.f);
    this->view_ = glm::mat4(1.f);
}

canvas::~canvas() = default;

void canvas::run()
{
    while (this->running_) {
        this->window_->clear();

        if (this->window_->update()) {
            this->running_ = false;
        }

        if (this->update_) {
            this->update_(this->nodes_);
        }

        if (this->gl_ctx_) {
            this->gl_ctx_->swap_buffers();
        }
    }
}

void canvas::on_update(update_fn cb)
{
    this->update_ = cb;
}

void canvas::close()
{
    this->running_ = false;
}

void canvas::draw_shape(ref<drawable> shape)
{
    this->nodes_.push_back(shape);
}

void canvas::draw_shape_at(ref<drawable> shape, int x, int y)
{
    this->nodes_.push_back(shape);
}

}