//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_CANVAS_HPP
#define MB2DC_CANVAS_HPP

#include "drawable/drawable.hpp"
#include "graphics/context.hpp"
#include "window.hpp"

namespace mb2dc {

using update_fn = std::function<void(const std::vector<ref<drawable_t>> &)>;

class canvas_t {
public:
    explicit canvas_t(const window_spec &spec);
    ~canvas_t();

    /*
     * Avoid dangling pointer's by making apps non copyable.
     */
    canvas_t(const canvas_t &) = delete;
    canvas_t &operator=(const canvas_t &) = delete;

    void run();
    void close();
    void on_update(update_fn cb);

    void draw_shape(ref<drawable_t> shape);
    void draw_shape_at(ref<drawable_t> shape, int x, int y);

    inline static canvas_t &get() { return *canvas_t::instance_; }

    ref<window_t> window_;
    glm::mat4 proj_, view_;

private:
    update_fn update_;
    ref<input_t> input_;
    ref<gl_context_t> gl_ctx_;
    bool running_, minimized_;
    std::vector<ref<drawable_t>> nodes_;

    static canvas_t *instance_;
};

}

#endif //MB2DC_CANVAS_HPP
