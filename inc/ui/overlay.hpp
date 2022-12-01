//
// Created by Garrett on 11/18/2022.
//

#ifndef MB2DC_OVERLAY_H
#define MB2DC_OVERLAY_H

#include <utility>

#include "core/draw_queue.hpp"
#include "font_manager.hpp"
#include "text.hpp"

namespace mb2dc {

using ui_update_fn = std::function<void(const std::vector<ui_element_t *> &elements, const glm::mat4 &view_proj)>;

class ui_overlay_t {
public:
    void update(const glm::mat4 &view_proj);
    inline void on_update(ui_update_fn cb) { this->update_ = std::move(cb); }

    ref<text_t> draw_text(std::string_view text,
                          const ref<font_t> &font = nullptr,
                          glm::vec2 pos = {0.f, 0.f},
                          float scale = 1.f);
    void draw_text(const ref<text_t> &text);
    void draw_element(const ref<ui_element_t> &element);

    void erase(ui_element_t *element);
    void erase(const ref<ui_element_t> &element);

    static ref<ui_overlay_t> get();

    /*
     * Returns whether or not the instance_ of ui_overlay_t has been created. Avoids some segfault
     * and/or infinite loop caused by calling the destructor of ui_element_t when the queue_ is
     * being created.
     */
    static bool initialized() { return init_; }

private:
    ui_overlay_t();

    font_manager_t *font_manager_;
    draw_queue_t<ui_element_t> queue_{};
    ui_update_fn update_;

    static ref<ui_overlay_t> instance_;
    static bool init_;
};

}

#endif //MB2DC_OVERLAY_H
