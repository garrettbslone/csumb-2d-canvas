//
// Created by Garrett on 11/25/21.
//

#include <core/input.hpp>
#include <GLFW/glfw3.h>

namespace mb2dc {

ref<input> input::instance_ = nullptr;

static GLFWwindow *g_native_win;

double input::get_mouse_x()
{
    return get_mouse_pos().second;
}

double input::get_mouse_y()
{
    return get_mouse_pos().first;
}

std::pair<double, double> input::get_mouse_pos()
{
    double x = 0., y = 0.;

    if (g_native_win) {
        glfwGetCursorPos(g_native_win, &x, &y);
    }

    return {x, y};
}

ref<input> input::get(void *w)
{
    if (w) {
        input i(w);
        input::instance_ = new_ref<input>(i);
    }

    return instance_;
}

input::input(void *w)
{
    if (w) {
        g_native_win = reinterpret_cast<GLFWwindow *>(w);
    }
}

}
