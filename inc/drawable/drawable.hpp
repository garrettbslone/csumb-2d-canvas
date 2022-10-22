//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_DRAWABLE_HPP
#define MB2DC_DRAWABLE_HPP

#include "graphics/buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

namespace mb2dc {

class drawable_t {
public:
    virtual ~drawable_t() = default;

    void draw() const;
    void translate(const glm::vec2 &v);
    void set_z_index(float z);
    void rotate(float deg, const glm::vec3 &axis = glm::vec3(0.f, 0.f, 1.f));
    void scale(const glm::vec2 &v);

    inline void reset_transformations() { this->model_ = glm::mat4(1.f); }
    inline glm::mat4 get_model_mat() { return this->model_; }

    topology topo_;
    ref<gl_shader_t> shader_;
    ref<gl_texture_t> texture_;
    ref<gl_index_buffer_t> ib_;
    ref<gl_vertex_buffer_t> vb_;
    ref<gl_vertex_array_t> va_;
    std::string name_;

protected:
    glm::mat4 model_;
};

}

#endif //MB2DC_DRAWABLE_HPP
