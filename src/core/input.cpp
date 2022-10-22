//
// Created by Garrett on 11/25/21.
//

#include <core/input.hpp>
#include <GLFW/glfw3.h>

namespace mb2dc {

ref<input_t> input_t::instance_ = nullptr;

static GLFWwindow *g_native_win;

double input_t::get_mouse_x()
{
    return get_mouse_pos().second;
}

double input_t::get_mouse_y()
{
    return get_mouse_pos().first;
}

std::pair<double, double> input_t::get_mouse_pos()
{
    double x = 0., y = 0.;

    if (g_native_win) {
        glfwGetCursorPos(g_native_win, &x, &y);
    }

    return {x, y};
}

ref<input_t> input_t::get(void *w)
{
    if (w) {
        input_t i(w);
        input_t::instance_ = new_ref<input_t>(i);
    }

    return instance_;
}

input_t::input_t(void *w)
{
    if (w) {
        g_native_win = reinterpret_cast<GLFWwindow *>(w);
    }
}

}
