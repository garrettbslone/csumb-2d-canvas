//
// Created by Garrett on 10/30/2021.
//

#ifndef MB2DC_RENDERER_HPP
#define MB2DC_RENDERER_HPP

#include "gl_util.hpp"
#include "core/util.hpp"

struct GLFWwindow;

namespace mb2dc {

/*
 * An OpenGL defined renderer.
 */
class gl_context_t {
public:
    explicit gl_context_t(void *w);
    ~gl_context_t() = default;

    gl_context_t(const gl_context_t &) = delete;
    gl_context_t &operator=(const gl_context_t &) = delete;

    void swap_buffers();

    static void state_enable(int state);
    static void state_disable(int state);

    static void set_depth(bool enable);
    static void cull_face(bool enable);
    static void blend(bool enable);

private:
    ref<::GLFWwindow *> native_window_;
};

}

#endif
