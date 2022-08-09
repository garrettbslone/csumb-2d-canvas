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
class gl_context {
public:
    explicit gl_context(void *w);
    ~gl_context() = default;

    gl_context(const gl_context &) = delete;
    gl_context &operator=(const gl_context &) = delete;

    void swap_buffers();

private:
    ::GLFWwindow * w_;
    ref<::GLFWwindow *> native_window_;
};

}

#endif //GLW_GL_RENDERER_HPP
