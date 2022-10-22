//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GL_RENDERER_HPP
#define GLW_GL_RENDERER_HPP

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

private:
    ref<::GLFWwindow *> native_window_;
};

}

#endif //GLW_GL_RENDERER_HPP
