//
// Created by Garrett on 10/22/2022.
//

#include <core/draw_queue.hpp>

#include <algorithm>

namespace mb2dc {

draw_queue_t::draw_queue_t()
    : draw_queue_t(-100.f, 100.f)
{

}

draw_queue_t::draw_queue_t(float front, float back)
    : front_(front), back_(back)
{
    this->shapes_ = {};
}

void draw_queue_t::enqueue(drawable_t *shape) const
{
    auto it = std::find_if(this->shapes_.begin(),
                           this->shapes_.end(),
                           [shape](drawable_t *curr)
                           {
                                return curr->z_index_ >= shape->z_index_;
                           });

    this->shapes_.insert(it, shape);
}

void draw_queue_t::dequeue(const drawable_t *shape) const
{
    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    if (it != this->shapes_.end()) {
        this->shapes_.erase(it);
    }
}

void draw_queue_t::adjust(drawable_t *shape, float z_index) const
{
    if (shape->z_index_ == z_index) {
        return;
    }

    shape->z_index_ = z_index;

    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    if (it != this->shapes_.end()) {
        auto end = std::find_if(this->shapes_.begin(),
                                  this->shapes_.end(),
                                  [&shape](drawable_t *curr)
                                  {
                                        return curr->z_index_ >= shape->z_index_ && curr != shape;
                                  });

        if (end != this->shapes_.end()) {
            std::rotate(it, it++, end);
        }
    }
}

void draw_queue_t::bring_to_front(const drawable_t *shape)
{
    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    // If the shape is already in the front, no need to move it. If it's not
    // in the queue then we have no work to do.
    if (*it != *this->shapes_.begin() && it != this->shapes_.end()) {
        std::rotate(this->shapes_.begin(), it, it + 1);
    }
}

void draw_queue_t::bring_to_back(const drawable_t *shape)
{
    auto pivot = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

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

const std::vector<drawable_t *> &draw_queue_t::get()
{
    return this->shapes_;
}

} // mb2dc