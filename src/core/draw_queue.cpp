//
// Created by Garrett on 10/22/2022.
//

#include <core/draw_queue.h>

#include <algorithm>

namespace mb2dc {

draw_queue_t::draw_queue_t()
    : draw_queue_t(-100, 100)
{

}

draw_queue_t::draw_queue_t(int front, int back)
    : front_(front), back_(back)
{
    this->shapes_ = {};
}

void draw_queue_t::enqueue(ref<drawable_t> shape)
{
    if (this->shapes_.empty()) {
        this->shapes_.push_back(shape);
    }

    auto it = this->shapes_.begin();

    while (it != this->shapes_.end()) {
        if ((*it)->z_index_ >= shape->z_index_) {
            break;
        }

        it++;
    }

    this->shapes_.insert(it, shape);
}

void draw_queue_t::dequeue(ref<drawable_t> shape)
{
    auto it = std::find_if(this->shapes_.begin(),
                              this->shapes_.end(),
                              [&shape] (const ref<drawable_t> &node) -> bool
                              {
                                  return shape == node;
                              });

    if (it != this->shapes_.end()) {
        this->shapes_.erase(it);
    }
}

void draw_queue_t::bring_to_front(ref<drawable_t> shape)
{
    auto it = std::find_if(this->shapes_.begin(),
                              this->shapes_.end(),
                              [&shape] (const ref<drawable_t> &node) -> bool
                              {
                                    return shape == node;
                              });

    // If the shape is already in the front, no need to move it. If it's not
    // in the queue then we have no work to do.
    if (*it != *this->shapes_.begin() && it != this->shapes_.end()) {
        std::rotate(this->shapes_.begin(), it, it + 1);
    }
}

void draw_queue_t::bring_to_back(ref<drawable_t> shape)
{
    auto pivot = std::find_if(this->shapes_.begin(),
                              this->shapes_.end(),
                              [&shape] (const ref<drawable_t> &node) -> bool
                              {
                                  return shape == node;
                              });
    // If the shape is already in the end, no need to move it. If it's not
    // in the queue then we have no work to do.
    if (*pivot != *this->shapes_.rbegin() && pivot != this->shapes_.end()) {
        std::rotate(this->shapes_.begin(), pivot, pivot + 1);
    }
}

void draw_queue_t::clear()
{
    this->shapes_.clear();
}

const std::vector<ref<drawable_t>> &draw_queue_t::get()
{
    return this->shapes_;
}

} // mb2dc