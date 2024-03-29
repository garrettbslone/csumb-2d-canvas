//
// Created by Garrett on 8/4/22.
//

#include <core/canvas.hpp>
#include <core/exception.hpp>
#include <graphics/shader.hpp>

#include <glm/ext.hpp>
#include <utility>

namespace mb2dc {

canvas_t *canvas_t::instance_ = nullptr;

canvas_t::canvas_t(const window_spec &spec)
{
    if (instance_) {
        throw canvas_ex("app already exists!");
    }

    instance_ = this;

    this->window_ = new_ref<window_t>(spec, window_data {
            nullptr, nullptr, nullptr, nullptr
    });
    this->running_ = true;
    this->minimized_ = false;

    this->gl_ctx_ = new_ref<gl_context_t>(this->window_->native_window());

    float w = this->window_->width() / 2.f, h  = this->window_->height() / 2.f;
    this->proj_ = glm::ortho(-w, w, -h, h, -100.f, 100.f);
    this->view_ = glm::mat4(1.f);

    this->overlay_ = ui_overlay_t::get();

    this->window_->on_resize([this] (int32_t width, int32_t height)
    {
        auto w = static_cast<float>(width);
        auto h = static_cast<float>(height);

        this->proj_ = glm::ortho(-w / 2, w / 2, -h / 2, h / 2, -100.f, 100.f);

        if (this->resize_) {
            this->resize_(width, height);
        }

        this->overlay_->resize_(width, height);
    });

    this->input_ = this->window_->data_.input_;

    this->input_->key_down_ = [&] (int key, int mods)
    {
        UNUSED(mods);

        if (static_cast<key_code>(key) == KEY_ESCAPE) {
            auto _win = reinterpret_cast<GLFWwindow *>(this->window_->native_window());
            glfwSetWindowShouldClose(_win, GLFW_TRUE);
        }
    };

    this->update_ = [&] (const std::vector<ref_t<drawable_t>> &nodes, const glm::mat4 &view_proj)
    {
        for (const auto& n: nodes) {
            n->draw(view_proj);
        }
    };

    this->register_ui_clicks();
}

canvas_t::~canvas_t() = default;

void canvas_t::run(bool event_driven)
{
    glm::mat4 view_proj;
    this->window_->event_driven_ = event_driven;

    while (this->running_) {
        this->window_->clear();

        if (this->window_->update()) {
            this->running_ = false;
        }

        if (this->update_) {
            view_proj = this->proj_ * this->view_;
            this->update_(this->queue_.get(), view_proj);
        }

        if (this->overlay_) {
            this->overlay_->update(view_proj);
        }

        if (this->gl_ctx_) {
            this->gl_ctx_->swap_buffers();
        }
    }
}

void canvas_t::on_update(update_fn cb)
{
    this->update_ = std::move(cb);
}

void canvas_t::on_window_resize(resize_fn cb)
{
    this->resize_ = std::move(cb);
}

void canvas_t::force_fullscreen()
{
    this->window_->force_fullscreen();
}

void canvas_t::on_close(close_fn cb)
{
    this->window_->on_close(std::move(cb));
}

void canvas_t::set_title(const std::string &title)
{
    this->window_->set_title(title);
}

void canvas_t::set_clear_color(const glm::vec4 &c)
{
    this->window_->clear_clr_ = c;
}

double canvas_t::aspect_ratio()
{
    return this->window_->aspect_ratio();
}

uint32_t canvas_t::width() const
{
    return this->window_->width();
}

uint32_t canvas_t::height() const
{
    return this->window_->height();
}

ref_t<text_t> canvas_t::draw_ui_text(std::string_view text, const ref_t<font_t> &font, glm::vec2 pos, float scale)
{
    return this->overlay_->draw_text(text, font, pos, scale);
}

ref_t<text_t> canvas_t::draw_ui_text(std::string_view text, const ref_t<font_t> &font, int16_t alignment, float scale)
{
    return this->overlay_->draw_text(text, font, alignment, scale);
}

void canvas_t::draw_ui_text(const ref_t<text_t> &text)
{
    this->overlay_->draw_text(text);
}

void canvas_t::draw_ui_element(const ref_t<ui_element_t> &element)
{
    this->overlay_->draw_element(element);
}

void canvas_t::erase_ui_element(const ref_t<ui_element_t> &element)
{
    this->overlay_->erase(element);
}

void canvas_t::on_ui_update(ui_update_fn cb)
{
    this->overlay_->on_update(std::move(cb));
}

glm::vec2 canvas_t::get_mouse_pos_real()
{
    return this->input_->get_mouse_pos_real();
}

glm::vec2 canvas_t::get_mouse_pos_rel()
{
    return this->input_->get_mouse_pos_rel(this->width(), this->height());
}

glm::vec2 canvas_t::get_mouse_pos_rel(uint32_t dim)
{
    return this->input_->get_mouse_pos_rel(dim);
}

glm::vec2 canvas_t::get_mouse_pos_rel(uint32_t width, uint32_t height)
{
    return this->input_->get_mouse_pos_rel(width, height);
}

void canvas_t::on_mouse_btn_down(mouse_button_down_fn cb)
{
    this->input_->mouse_btn_down_ = std::move(cb);
    this->register_ui_clicks();
}

void canvas_t::on_mouse_btn_up(mouse_button_up_fn cb)
{
    this->input_->mouse_btn_up_ = std::move(cb);
}

void canvas_t::on_mouse_move(mouse_move_fn cb)
{
    this->input_->mouse_move_ = std::move(cb);
}

void canvas_t::on_key_down(key_down_fn cb)
{
    this->input_->key_down_ = std::move(cb);
}

void canvas_t::on_key_up(key_up_fn cb)
{
    this->input_->key_up_ = std::move(cb);
}

bool canvas_t::get_key(const key_code &k)
{
    return this->input_->get_key(k);
}

bool canvas_t::get_key_down(const key_code &k)
{
    return this->input_->get_key_down(k);
}

bool canvas_t::get_key_up(const key_code &k)
{
    return this->input_->get_key_up(k);
}

void canvas_t::register_ui_clicks()
{
    this->overlay_->click_thru(this->input_->mouse_btn_down_);
    this->input_->mouse_btn_down_ = [this] (int btn)
    {
        auto pos = this->input_->get_mouse_pos_rel(this->window_->width(), this->window_->height());
        this->overlay_->try_click(btn, pos.x, pos.y);
    };
}

void canvas_t::close()
{
    this->running_ = false;
}

void canvas_t::fixed_size()
{
    this->window_->set_resizable(false);
}

void canvas_t::resizable()
{
    this->window_->set_resizable(true);
}

void canvas_t::draw_shape(const ref_t<drawable_t>& shape)
{
    this->queue_.enqueue(shape);
}

void canvas_t::draw_shape_at(const ref_t<drawable_t>& shape, int x, int y)
{
    shape->translate({x, y});
    this->queue_.enqueue(shape);
}

}