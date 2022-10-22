//
// Created by Garrett on 10/30/2021.
//

#include <graphics/context.hpp>

#include <GLFW/glfw3.h>

namespace mb2dc {

gl_context_t::gl_context_t(void *w)
{
    this->native_window_ = new_ref<GLFWwindow *>(reinterpret_cast<GLFWwindow *>(w));

    if (this->native_window_ && this->native_window_.get()) {
        glfwMakeContextCurrent(*this->native_window_);
    }
}

void gl_context_t::swap_buffers()
{
    if (this->native_window_ && this->native_window_.get()) {
        glfwSwapBuffers(*this->native_window_);
    }
}

}
