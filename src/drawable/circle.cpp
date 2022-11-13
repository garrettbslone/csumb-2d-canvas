//
// Created by Garrett on 12/7/2021.
//

#include <drawable/circle.hpp>
#include <glm/gtc/constants.hpp>

namespace mb2dc {

int circle_t::created_ = 0;
int circle_t::alive_ = 0;

circle_t::circle_t(int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 std::vector<glm::vec4>(vertex_count, {1.f, 1.f, 1.f, 1.f}),
                 vertex_count,
                 r);
}

circle_t::circle_t(const std::string &name, int vertex_count, int r)
{
    this->create(name,
                 std::vector<glm::vec4>(vertex_count, {1.f, 1.f, 1.f, 1.f}),
                 vertex_count,
                 r);
}

circle_t::circle_t(const glm::vec4 &color, int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 std::vector<glm::vec4>(vertex_count, color),
                 vertex_count,
                 r);
}

circle_t::circle_t(const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 colors,
                 vertex_count,
                 r);
}

circle_t::circle_t(const std::string &name, const glm::vec4 &color, int vertex_count, int r)
{
    this->create(name,
                 std::vector<glm::vec4>(vertex_count, color),
                 vertex_count,
                 r);
}

circle_t::circle_t(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    this->create(name,
                 colors,
                 vertex_count,
                 r);
}

circle_t::~circle_t()
{
    alive_--;
}

void circle_t::create(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    if (colors.size() < vertex_count) {
        throw drawable_ex("There must be one color for each vertex ("  + std::to_string(vertex_count) +
                            "). Only " + std::to_string(colors.size()) + " given.");
    }

    created_++;
    alive_++;

    std::vector<vertex_t> verts;

    double slice = 2 * glm::pi<double>() / vertex_count;

    for (auto i = 0; i < vertex_count; i++) {
        double rads = i * slice;

        double x = r * glm::cos(rads), y = r * glm::sin(rads);
        vertex_t v = {
                glm::vec2(x, y),
                glm::vec4(1.f, 1.f, 1.f, 1.f),
                glm::vec2((x / r + 1) * 0.5, (y / r + 1) * 0.5)
        };
        verts.push_back(v);
    }

    this->vb_ = new_ref<gl_vertex_buffer_t>(verts);
    this->ib_ = nullptr;

    this->va_ = new_ref<gl_vertex_array_t>();
    this->va_->set_vertex_buffer(this->vb_);

    this->topo_ = TRIANGLE_FAN;
    this->model_ = glm::mat4(1.f);
    this->textures_ = {};
    this->shader_ = gl_shader_t::get_default();

    this->name_ = name;
}

}