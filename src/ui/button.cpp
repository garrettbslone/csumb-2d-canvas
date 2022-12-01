//
// Created by Garrett on 11/12/22.
//

#include <ui/button.hpp>

namespace mb2dc {

button_t::button_t()
{

}

button_t::button_t(drawable_t *base)
{

}

void button_t::set_state(uint16_t state, bool clear_previous)
{
    if (clear_previous) {
        this->state_ = 0;
    }

    this->state_ |= state;
}

}