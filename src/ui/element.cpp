//
// Created by Garrett on 11/30/2022.
//

#include <ui/element.hpp>
#include <ui/overlay.hpp>

namespace mb2dc {

int clickable_t::ID = 0;

clickable_t::clickable_t()
{
    this->id_ = ID++;
}

}