//
// Created by Garrett on 11/12/22.
//

#include <core/canvas.hpp>

#include <ui/button.hpp>
#include <ui/overlay.hpp>

namespace mb2dc {

button_t::button_t(glm::vec4 border_color, float border_radius)
        : border_color_(border_color)
{
    this->set_state(STATE_INACTIVE | STATE_UNCLICKED | STATE_VISIBLE, false);
    this->set_border_radius(border_radius);

    std::vector<vertex_t> verts = {
            {
                glm::vec2{-50.0f, 10.0f},
                glm::vec4{0.8f, 0.8f, 0.8f, 1.f},
                glm::vec2{0.0f, 1.0f}
            },
            {
                glm::vec2{-50.0f, -10.0f},
                glm::vec4{0.8f, 0.8f, 0.8f, 1.f},
                glm::vec2{0.0f, 0.0f}
            },
            {
                glm::vec2{50.0f, 10.0f},
                glm::vec4{0.8f, 0.8f, 0.8f, 1.f},
                glm::vec2{1.0f, 1.0f}
            },
            {
                glm::vec2{50.0f, -10.0f},
                glm::vec4{0.8f, 0.8f, 0.8f, 1.f},
                glm::vec2{1.0f, 0.0f}
            }
    };
    this->vb_ = new_ref<gl_vertex_buffer_t>(verts);
    std::vector<uint32_t> inds = {
            0, 1, 2,
            1, 3, 2
    };
    this->ib_ = new_ref<gl_index_buffer_t>(inds);

    this->va_ = new_ref<gl_vertex_array_t>();
    this->va_->set_vertex_buffer(this->vb_);
    if (this->ib_) {
        this->va_->set_index_buffer(this->ib_);
    }

    this->topo_ = TRIANGLES;
    this->model_ = glm::mat4(1.f);
    this->textures_ = {};
    this->shader_ = new_ref<gl_shader_t>("button", get_res("shaders/flat.vs.glsl"), get_res("shaders/button.fs.glsl"));

    this->name_ = "button-" + std::to_string(this->id_);
}

void button_t::draw(const glm::mat4 &view_proj) const
{
    static constexpr glm::vec2 BORDER_RADIUS_SCALE_FACTOR = {13.5f, 50.f};

    this->shader_->bind();
    this->shader_->set_uniform_float2("uCenter", this->center_);
    this->shader_->set_uniform_float2("uBorderRadius", this->scale_ / (BORDER_RADIUS_SCALE_FACTOR * this->border_radius_));
    this->shader_->set_uniform_float4("uBorderColor", this->border_color_);

    drawable_t::draw(view_proj);
}

glm::vec2 button_t::convert_to_spos(const glm::vec2 &v)
{
    auto c = canvas_t::get();

    if (!c) {
        return {-1, -1};
    }

    double w = c->window_->width() / 2.0, h = c->window_->height() / 2.0;

    // base scale button with no base is bounded on a box of [-1, 1]
    // x = -1 / scale, y = 1 / scale
    return glm::vec2(w, h) * (v / this->scale_) / glm::vec2(1.f);
}

/*
 * Very naive approach assuming that the first 2 elements in the vertex buffer will be the [x, y]
 * of the top left of the button.
 */
bool button_t::overlapping(double x, double y)
{
    auto verts = this->vb_->get_data();
    auto center = convert_to_spos(this->center_);
    auto top_left = center + glm::vec2{verts[0], verts[1]} * this->scale_;

    auto w = abs(center.x - top_left.x) * 2.0;
    auto h = abs(top_left.y - center.y) * 2.0;

    return x >= top_left.x && x <= (top_left.x + w) && y <= top_left.y && y >= (top_left.y - h);
}

void button_t::set_border_radius(float border_radius)
{
    if (border_radius >= 0.f) {
        border_radius_ = border_radius;
    }
}

}