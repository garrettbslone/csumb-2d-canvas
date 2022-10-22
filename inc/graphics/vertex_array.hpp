//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_GL_VERTEX_ARRAY_HPP
#define GLW_GL_VERTEX_ARRAY_HPP

#include "buffer.hpp"
#include "core/util.hpp"

namespace mb2dc {

/*
 * An OpenGL defined vertex_array.
 */
class gl_vertex_array : public gl_object {
public:
    explicit gl_vertex_array();
    ~gl_vertex_array() override;

    void bind() const override;
    void unbind() const override;

    void set_vertex_buffer(ref<gl_vertex_buffer> buff);
    void set_index_buffer(ref<gl_index_buffer> buff);

    inline ref<gl_vertex_buffer> get_vertex_buffers() const { return this->vb_; }
    inline ref<gl_index_buffer> get_index_buffer() const { return this->ib_; }

private:
    /*
     * v_buff_index_: the index of the next attribute to be bound.
     */
    uint32_t v_buff_index_ = 0, offset_ = 0;
    ref<gl_vertex_buffer> vb_{};
    ref<gl_index_buffer> ib_{};
};

}

#endif //GLW_GL_VERTEX_ARRAY_HPP
