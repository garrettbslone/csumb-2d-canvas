//
// Created by Garrett on 11/25/21.
//

#ifndef GLW_INPUT_HPP
#define GLW_INPUT_HPP

#include "key_codes.hpp"
#include "util.hpp"

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
    input_t() = default;
    virtual ~input_t() = default;

    static double get_mouse_x();
    static double get_mouse_y();
    static std::pair<double, double> get_mouse_pos();

    key_down_fn key_down_;
    key_up_fn key_up_;
    mouse_button_down_fn mouse_btn_down_;
    mouse_button_up_fn mouse_btn_up_;
    mouse_move_fn mouse_move_;

    static ref<input_t> get(void *w);

protected:
    explicit input_t(void *w);

    static ref<input_t> instance_;
};

}

#endif //GLW_INPUT_HPP
