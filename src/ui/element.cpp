//
// Created by Garrett on 11/30/2022.
//

#include <ui/element.hpp>
#include <ui/overlay.hpp>

namespace mb2dc {

ui_element_t::~ui_element_t()
{
    if (ui_overlay_t::initialized()) {
        auto overlay = ui_overlay_t::get();

        if (overlay) {
            overlay->erase(this);
        }
    }
}

int clickable_t::ID = 0;

clickable_t::clickable_t()
{
    this->id_ = ID++;
}

}