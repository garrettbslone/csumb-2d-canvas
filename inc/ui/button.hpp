//
// Created by Garrett on 11/12/22.
//

#ifndef MB2DC_BUTTON_HPP
#define MB2DC_BUTTON_HPP

#include "element.hpp"

#include <functional>

namespace mb2dc {

class button_t;

using btn_click_fn = std::function<void(button_t *thiz)>;

class button_t : public ui_element_t {
public:
    static constexpr uint16_t STATE_ACTIVE = 1 << 0;
    static constexpr uint16_t STATE_INACTIVE = 1 << 2;
    static constexpr uint16_t STATE_VISIBLE = 1 << 3;
    static constexpr uint16_t STATE_INVISIBLE = 1 << 4;
    static constexpr uint16_t STATE_CLICKED = 1 << 5;
    static constexpr uint16_t STATE_UNCLICKED = 1 << 6;

    button_t();
    explicit button_t(drawable_t *base);

    inline void on_click(const btn_click_fn &cb) { this->click_ = cb; }

    void set_state(uint16_t state, bool clear_previous);
    inline uint16_t state() const { return this->state_; }

private:
    btn_click_fn click_;

    uint16_t state_;
};

}

#endif //MB2DC_BUTTON_HPP
