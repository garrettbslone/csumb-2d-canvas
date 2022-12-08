//
// Created by Garrett on 10/30/2021.
//

#include <graphics/context.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace mb2dc {

gl_context_t::gl_context_t(void *w)
{
    this->native_window_ = new_ref<GLFWwindow *>(reinterpret_cast<GLFWwindow *>(w));

    if (this->native_window_ && this->native_window_.get()) {
        glfwMakeContextCurrent(*this->native_window_);
    }

    set_depth(false);
    cull_face(true);
    blend(true);
}

void gl_context_t::swap_buffers()
{
    if (this->native_window_ && this->native_window_.get()) {
        glfwSwapBuffers(*this->native_window_);
    }
}

void gl_context_t::state_enable(int state)
{
    glEnable(state);
}

void gl_context_t::state_disable(int state)
{
    glDisable(state);
}

void gl_context_t::set_depth(bool enable)
{
    if (enable) {
        state_enable(GL_DEPTH_TEST);
    } else {
        state_disable(GL_DEPTH_TEST);
    }
}

void gl_context_t::cull_face(bool enable)
{
    if (enable) {
        state_enable(GL_CULL_FACE);
    } else {
        state_disable(GL_CULL_FACE);
    }

    glCullFace(GL_BACK);
}

void gl_context_t::blend(bool enable)
{
    if (enable) {
        state_enable(GL_BLEND);
    } else {
        state_disable(GL_BLEND);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}
