//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_CANVAS_HPP
#define MB2DC_CANVAS_HPP

#include "draw_queue.h"
#include "graphics/context.hpp"
#include "window.hpp"

namespace mb2dc {

using update_fn = std::function<void(const std::vector<drawable_t *> &shapes, const glm::mat4 &view_proj)>;

class canvas_t {
public:
    explicit canvas_t(const window_spec &spec);
    ~canvas_t();

    /*
     * Avoid dangling pointer's by making apps non copyable.
     */
    canvas_t(const canvas_t &) = delete;
    canvas_t &operator=(const canvas_t &) = delete;

    /*
     * If wait_events is set to true, then the canvas's on_update will only be called
     * once per
     */
    void run(bool wait_events = false);
    void close();
    void on_update(update_fn cb);
    void on_window_resize(resize_fn cb);

    void fixed_size();
    void resizable();

    void draw_shape(ref<drawable_t> shape);
    void draw_shape_at(ref<drawable_t> shape, int x, int y);

    inline static const canvas_t *get() { return canvas_t::instance_; }

    ref<window_t> window_;
    glm::mat4 proj_, view_;

private:
    resize_fn resize_;
    update_fn update_;
    ref<input_t> input_;
    ref<gl_context_t> gl_ctx_;
    bool running_, minimized_;
    draw_queue_t queue_;

    static canvas_t *instance_;

    friend drawable_t;
};

}

#endif //MB2DC_CANVAS_HPP
