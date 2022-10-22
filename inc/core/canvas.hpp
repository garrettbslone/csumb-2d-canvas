//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_CANVAS_HPP
#define MB2DC_CANVAS_HPP

#include "drawable/drawable.hpp"
#include "graphics/context.hpp"
#include "window.hpp"

namespace mb2dc {

using update_fn = std::function<void(const std::vector<ref<drawable>> &)>;

class canvas {
public:
    explicit canvas(const window_spec &spec);
    ~canvas();

    /*
     * Avoid dangling pointer's by making apps non copyable.
     */
    canvas(const canvas &) = delete;
    canvas &operator=(const canvas &) = delete;

    void run();
    void close();
    void on_update(update_fn cb);

    void draw_shape(ref<drawable> shape);
    void draw_shape_at(ref<drawable> shape, int x, int y);

    inline static canvas &get() { return *canvas::instance_; }

    ref<window> window_;
    glm::mat4 proj_, view_;

private:
    update_fn update_;
    ref<input> input_;
    ref<gl_context> gl_ctx_;
    bool running_, minimized_;
    std::vector<ref<drawable>> nodes_;

    static canvas *instance_;
};

}

#endif //MB2DC_CANVAS_HPP
