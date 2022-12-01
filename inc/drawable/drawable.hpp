//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_DRAWABLE_HPP
#define MB2DC_DRAWABLE_HPP

#include "graphics/buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include <unordered_map>

namespace mb2dc {

class drawable_t {
public:
    ~drawable_t();

    void draw() const;

    virtual void draw(const glm::mat4 &view_proj) const;
    void translate(const glm::vec2 &v);
    void rotate(float deg, const glm::vec3 &axis = glm::vec3(0.f, 0.f, -1.f));
    void scale(const glm::vec2 &v);

    inline void reset_transformations() { this->model_ = glm::mat4(1.f); }
    inline glm::mat4 get_model_mat() { return this->model_; }

    void set_z_index(float z);
    inline float get_z_index() const { return this->z_index_; }

    bool is_slot_active(uint8_t slot);
    bool set_slot_active(uint8_t slot);
    uint8_t find_next_slot();
    inline uint64_t get_active_slots() const { return this->active_slots_; }

    topology topo_;
    ref<gl_shader_t> shader_;
    std::unordered_map<std::string, ref<gl_texture_t>> textures_;
    ref<gl_index_buffer_t> ib_;
    ref<gl_vertex_buffer_t> vb_;
    ref<gl_vertex_array_t> va_;
    std::string name_;

protected:
    glm::mat4 model_{1.f};
    float z_index_{0};

    int queue_pos_;

    /*
     * There are at least 48 texture slots in OpenGL, so we can keep track of the active ones
     * per drawable. Each drawable will have its own render call and slots can be shared between
     * different drawables.
     */
    uint64_t active_slots_{0x00000000};

    // forward declare the canvas_t here so that the draw_queue_t can be reordered
    // when a drawable changes its z_index_ (if needed)
    template<class>
    friend class draw_queue_t;
};

}

#endif //MB2DC_DRAWABLE_HPP
