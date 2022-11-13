//
// Created by Garrett on 12/6/2021.
//

#include <drawable/rect.hpp>
#include <graphics/gl_util.hpp>

namespace mb2dc {

int rect_t::created_ = 0;
int rect_t::alive_ = 0;

rect_t::rect_t()
{
    this->create("rect " + std::to_string(created_), std::vector<glm::vec4>(4, {0.f, 0.f, 0.f, 1.f}));
}

rect_t::rect_t(const std::string &name)
{
    this->create(name, std::vector<glm::vec4>(4, {0.f, 0.f, 0.f, 1.f}));
}

rect_t::rect_t(const glm::vec4 &color)
{
    this->create("rect " + std::to_string(created_), {color, color, color, color});
}

rect_t::rect_t(const std::string &name, const glm::vec4 &color)
{
    this->create(name, {color, color, color, color});
}

rect_t::rect_t(const std::vector<glm::vec4> &colors)
{
    this->create("rect " + std::to_string(created_), colors);
}

rect_t::rect_t(const std::string &name, const std::vector<glm::vec4> &colors)
{
    this->create(name, colors);
}

rect_t::~rect_t()
{
    alive_--;
}

void rect_t::create(const std::string &name, const std::vector<glm::vec4> &colors)
{
    if (colors.size() < 4) {
        throw drawable_ex("There must be one color for each vertex (4). Only "
            + std::to_string(colors.size()) + " given.");
    }

    created_++;
    alive_++;

    std::vector<vertex_t> verts = {
            {
                    glm::vec2{-100.0f, 100.0f},
                    colors[0],
                    glm::vec2{0.0f, 1.0f}
            },
            {
                    glm::vec2{-100.0f, -100.0f},
                    colors[1],
                    glm::vec2{0.0f, 0.0f}
            },
            {
                    glm::vec2{100.0f, 100.0f},
                    colors[2],
                    glm::vec2{1.0f, 1.0f}
            },
            {
                    glm::vec2{100.0f, -100.0f},
                    colors[3],
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
    this->va_->set_index_buffer(this->ib_);
    this->va_->set_vertex_buffer(this->vb_);

    this->topo_ = TRIANGLES;
    this->model_ = glm::mat4(1.f);
    this->textures_ = {};
    this->shader_ = gl_shader_t::get_default();

    this->name_ = name;
}

}
