//
// Created by Garrett on 12/7/2021.
//

#include <drawable/circle.hpp>
#include <glm/gtc/constants.hpp>

namespace mb2dc {

int circle::created_ = 0;
int circle::alive_ = 0;

circle::circle(int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 std::vector<glm::vec4>(vertex_count, {1.f, 1.f, 1.f, 1.f}),
                 vertex_count,
                 r);
}

circle::circle(const std::string &name, int vertex_count, int r)
{
    this->create(name,
                 std::vector<glm::vec4>(vertex_count, {1.f, 1.f, 1.f, 1.f}),
                 vertex_count,
                 r);
}

circle::circle(const glm::vec4 &color, int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 std::vector<glm::vec4>(vertex_count, color),
                 vertex_count,
                 r);
}

circle::circle(const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    this->create("circle " + std::to_string(created_),
                 colors,
                 vertex_count,
                 r);
}

circle::circle(const std::string &name, const glm::vec4 &color, int vertex_count, int r)
{
    this->create(name,
                 std::vector<glm::vec4>(vertex_count, color),
                 vertex_count,
                 r);
}

circle::circle(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    this->create(name,
                 colors,
                 vertex_count,
                 r);
}

circle::~circle()
{
    alive_--;
}

void circle::create(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r)
{
    if (colors.size() < vertex_count) {
        throw drawable_ex("There must be one color for each vertex ("  + std::to_string(vertex_count) +
                            "). Only " + std::to_string(colors.size()) + " given.");
    }

    created_++;
    alive_++;

    std::vector<vertex> verts;

    double slice = 2 * glm::pi<double>() / vertex_count;

    for (auto i = 0; i < vertex_count; i++) {
        double rads = i * slice;
        vertex v = {
                glm::vec2(r * glm::cos(rads), r * glm::sin(rads)),
                glm::vec4(1.f, 1.f, 1.f, 1.f),
                glm::vec2(0.f, 0.f)
        };
        verts.push_back(v);
    }

    this->vb_ = new_ref<gl_vertex_buffer>(verts);
    this->ib_ = nullptr;

    this->va_ = new_ref<gl_vertex_array>();
    this->va_->set_vertex_buffer(this->vb_);

    this->topo_ = TRIANGLE_FAN;
    this->model_ = glm::mat4(1.f);

    auto &defs = gl_shader::defaults;
    this->shader_ = new_ref<gl_shader>(defs[0], defs[1], defs[2]);

    this->name_ = name;
}

}