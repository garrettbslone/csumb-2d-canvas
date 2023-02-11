//
// Created by Garrett on 11/25/21.
//

#ifndef MB2DC_INPUT_HPP
#define MB2DC_INPUT_HPP

#include "key_codes.hpp"
#include "util.hpp"

#include <glm/vec2.hpp>

#include <functional>

namespace mb2dc {

using key_down_fn = std::function<void(int)>;
using key_up_fn = std::function<void(int)>;
using mouse_button_down_fn = std::function<void(int)>;
using mouse_button_up_fn = std::function<void(int)>;
using mouse_move_fn = std::function<void(double, double)>;

/*
 * A class that handles mouse & keyboard io.
 */
class input_t {
public:
    virtual ~input_t() = default;

    key_down_fn key_down_;
    key_up_fn key_up_;
    mouse_button_down_fn mouse_btn_down_;
    mouse_button_up_fn mouse_btn_up_;
    mouse_move_fn mouse_move_;

    /*
     * Get the (real) mouse position on the screen with [0, 0] in the top left
     * corner and [window width, window_height] in the bottom right corner.
     */
    static glm::vec2 get_mouse_pos_real();
    /*
     * Get the mouse position relative to the center of the window. [0, 0] is
     * in the center of the window, and the current width x height of the window
     * is used.
     */
    static glm::vec2 get_mouse_pos_rel();
    /*
     * Get the mouse position relative to the dim with [0, 0] in the center of the window.
     */
    static glm::vec2 get_mouse_pos_rel(uint32_t dim);
    /*
     * Get the mouse position relative to the width x height with [0, 0] in the
     * center of the window.
     */
    static glm::vec2 get_mouse_pos_rel(uint32_t width, uint32_t height);

    /*
     * The get_mouse_x_*** & get_mouse_y_*** functions work the same as the
     * get_mouse_pos_*** set of functions, but only returns either the x or y
     * coordinate.
     */
    static double get_mouse_x_real();
    static double get_mouse_y_real();
    static double get_mouse_x_rel();
    static double get_mouse_y_rel();
    static double get_mouse_x_rel(uint32_t dim);
    static double get_mouse_y_rel(uint32_t dim);
    static double get_mouse_x_rel(uint32_t width, uint32_t height);
    static double get_mouse_y_rel(uint32_t width, uint32_t height);

    static bool get_key(const key_code &k);
    static bool get_key_down(const key_code &k);
    static bool get_key_up(const key_code &k);
    static bool shifting();
    static bool ctrl();
    static bool alt();
    static bool caps_locked();

    static ref<input_t> get(void *w);

protected:
    explicit input_t(void *w);

    static ref<input_t> instance_;
};

}

#endif
