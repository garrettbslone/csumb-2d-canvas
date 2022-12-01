//
// Created by Garrett on 10/22/2022.
//

#ifndef MB2DC_DRAW_QUEUE_HPP
#define MB2DC_DRAW_QUEUE_HPP

#include "drawable/drawable.hpp"

#include <algorithm>

namespace mb2dc {

template<class T>
class draw_queue_t {
public:
    draw_queue_t();
    draw_queue_t(float front, float back);

    void enqueue(T *shape) const;
    void dequeue(const T *shape) const;

    void adjust(T *shape, float z_index) const;

    void bring_to_front(const T *shape);
    void bring_to_back(const T *shape);

    void clear();
    const std::vector<T *> &get();

private:
    mutable float front_, back_;

    mutable std::vector<T *> shapes_;
};

template<class T>
draw_queue_t<T>::draw_queue_t()
        : draw_queue_t(-100.f, 100.f)
{

}

template<class T>
draw_queue_t<T>::draw_queue_t(float front, float back)
        : front_(front), back_(back)
{
    this->shapes_ = {};
}

template<class T>
void draw_queue_t<T>::enqueue(T *shape) const
{
    auto it = std::find_if(this->shapes_.begin(),
                           this->shapes_.end(),
                           [shape](T *curr)
                           {
                               return curr->z_index_ >= shape->z_index_;
                           });

    this->shapes_.insert(it, shape);
}

template<class T>
void draw_queue_t<T>::dequeue(const T *shape) const
{
    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    if (it != this->shapes_.end()) {
        this->shapes_.erase(it);
    }
}

template<class T>
void draw_queue_t<T>::adjust(T *shape, float z_index) const
{
    if (shape->z_index_ == z_index) {
        return;
    }

    shape->z_index_ = z_index;

    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    if (it != this->shapes_.end()) {
        auto end = std::find_if(this->shapes_.begin(),
                                this->shapes_.end(),
                                [&shape](T *curr)
                                {
                                    return curr->z_index_ >= shape->z_index_ && curr != shape;
                                });

        if (end != this->shapes_.end()) {
            std::rotate(it, it++, end);
        }
    }
}

template<class T>
void draw_queue_t<T>::bring_to_front(const T *shape)
{
    auto it = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    // If the shape is already in the front, no need to move it. If it's not
    // in the queue then we have no work to do.
    if (*it != *this->shapes_.begin() && it != this->shapes_.end()) {
        std::rotate(this->shapes_.begin(), it, it + 1);
    }
}

template<class T>
void draw_queue_t<T>::bring_to_back(const T *shape)
{
    auto pivot = std::find(this->shapes_.begin(), this->shapes_.end(), shape);

    // If the shape is already in the end, no need to move it. If it's not
    // in the queue then we have no work to do.
    if (*pivot != *this->shapes_.rbegin() && pivot != this->shapes_.end()) {
        std::rotate(this->shapes_.begin(), pivot, pivot + 1);
    }
}

template<class T>
void draw_queue_t<T>::clear()
{
    this->shapes_.clear();
}

template<class T>
const std::vector<T *> &draw_queue_t<T>::get()
{
    return this->shapes_;
}


} // mb2dc

#endif //MB2DC_DRAW_QUEUE_HPP
