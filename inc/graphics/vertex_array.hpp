//
// Created by Garrett on 10/27/2021.
//

#ifndef MB2DC_VERTEX_ARRAY_HPP
#define MB2DC_VERTEX_ARRAY_HPP

#include "buffer.hpp"
#include "core/util.hpp"

namespace mb2dc {

/*
 * An OpenGL defined vertex_array.
 */
class gl_vertex_array_t : public gl_object_t {
public:
    explicit gl_vertex_array_t();
    ~gl_vertex_array_t() override;

    void bind() override;
    void unbind() const override;

    void set_vertex_buffer(ref_t<gl_vertex_buffer_t> buff);
    void set_index_buffer(ref_t<gl_index_buffer_t> buff);

    inline ref_t<gl_vertex_buffer_t> get_vertex_buffers() const { return this->vb_; }
    inline ref_t<gl_index_buffer_t> get_index_buffer() const { return this->ib_; }

private:
    ref_t<gl_vertex_buffer_t> vb_{};
    ref_t<gl_index_buffer_t> ib_{};
};

}

#endif
