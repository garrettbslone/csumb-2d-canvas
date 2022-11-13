//
// Created by Garrett on 11/25/21.
//

#include <core/input.hpp>
#include <GLFW/glfw3.h>

namespace mb2dc {

ref<input_t> input_t::instance_ = nullptr;

static GLFWwindow *g_native_win;

glm::vec2 input_t::get_mouse_pos_real()
{
    double x = 0., y = 0.;

    if (g_native_win) {
        glfwGetCursorPos(g_native_win, &x, &y);
    }

    return {x, y};
}

glm::vec2 input_t::get_mouse_pos_rel()
{
    int w = 0, h = 0;

    if (g_native_win) {
        glfwGetWindowSize(g_native_win, &w, &h);
    }

    return get_mouse_pos_rel(w, h);
}

glm::vec2 input_t::get_mouse_pos_rel(uint32_t dim)
{
    return get_mouse_pos_rel(dim, dim);
}

glm::vec2 input_t::get_mouse_pos_rel(uint32_t width, uint32_t height)
{
    double _w = width / 2.0, _h = height / 2.0;

    auto coord = get_mouse_pos_real();

    if (width != 0 && height != 0) {
        coord.x -= _w;
        coord.y = _h - coord.y;
    }

    return coord;
}

double input_t::get_mouse_x_real()
{
    return get_mouse_pos_real().x;
}

double input_t::get_mouse_y_real()
{
    return get_mouse_pos_real().y;
}

double input_t::get_mouse_x_rel()
{
    return get_mouse_pos_rel().x;
}

double input_t::get_mouse_y_rel()
{
    return get_mouse_pos_rel().y;
}

double input_t::get_mouse_x_rel(uint32_t dim)
{
    return get_mouse_pos_rel(dim).x;
}

double input_t::get_mouse_y_rel(uint32_t dim)
{
    return get_mouse_pos_rel(dim).y;
}

double input_t::get_mouse_x_rel(uint32_t width, uint32_t height)
{
    return get_mouse_pos_rel(width, height).x;
}

double input_t::get_mouse_y_rel(uint32_t width, uint32_t height)
{
    return get_mouse_pos_rel(width, height).y;
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
