//
// Created by Garrett on 10/22/2022.
//

#ifndef MB2DC_DRAW_QUEUE_H
#define MB2DC_DRAW_QUEUE_H

#include "drawable/drawable.hpp"

namespace mb2dc {

class draw_queue_t {
public:
    draw_queue_t();
    draw_queue_t(int front, int back);

    void enqueue(ref<drawable_t> shape);
    void dequeue(ref<drawable_t> shape);

    void bring_to_front(ref<drawable_t> shape);
    void bring_to_back(ref<drawable_t> shape);

    void clear();
    const std::vector<ref<drawable_t>> &get();

private:
    int front_, back_;

    std::vector<ref<drawable_t>> shapes_;
};

} // mb2dc

#endif //MB2DC_DRAW_QUEUE_H
