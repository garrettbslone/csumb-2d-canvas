//
// Created by Garrett on 10/22/2022.
//

#ifndef MB2DC_DRAW_QUEUE_HPP
#define MB2DC_DRAW_QUEUE_HPP

#include "drawable/drawable.hpp"

namespace mb2dc {

class draw_queue_t {
public:
    draw_queue_t();
    draw_queue_t(float front, float back);

    void enqueue(drawable_t *shape) const;
    void dequeue(const drawable_t *shape) const;

    void adjust(drawable_t *shape, float z_index) const;

    void bring_to_front(const drawable_t *shape);
    void bring_to_back(const drawable_t *shape);

    void clear();
    const std::vector<drawable_t *> &get();

private:
    mutable float front_, back_;

    mutable std::vector<drawable_t *> shapes_;
};

} // mb2dc

#endif //MB2DC_DRAW_QUEUE_HPP
