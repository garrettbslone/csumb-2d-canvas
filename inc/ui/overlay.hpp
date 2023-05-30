//
// Created by Garrett on 11/18/2022.
//

#ifndef MB2DC_OVERLAY_H
#define MB2DC_OVERLAY_H

#include "core/draw_queue.hpp"
#include "core/input.hpp"
#include "font_manager.hpp"
#include "text.hpp"

#include <unordered_set>

namespace mb2dc {

using ui_update_fn = std::function<void(
    const std::vector<ref_t<ui_element_t>> &elements,
    const glm::mat4 &view_proj)>;

class ui_overlay_t {
public:
    void update(const glm::mat4 &view_proj);
    inline void on_update(ui_update_fn cb) { this->update_ = std::move(cb); }

    void try_click(int btn, double x, double y);
    void click_thru(mouse_button_down_fn cb);

    ref_t<text_t> draw_text(std::string_view text,
                            const ref_t<font_t> &font = nullptr,
                            glm::vec2 pos = {0.f, 0.f},
                            float scale = 1.f);
    ref_t<text_t> draw_text(std::string_view text,
                            const ref_t<font_t> &font = nullptr,
                            int16_t alignment = align::HZ_LEFT | align::VT_TOP,
                            float scale = 1.f);
    void draw_text(const ref_t<text_t> &text);
    void draw_element(const ref_t<ui_element_t> &element);

    void erase(const ref_t<ui_element_t> &element);

    [[nodiscard]] static glm::vec2 calculate_alignment(uint16_t alignment,
                                                       float width,
                                                       float height,
                                                       float buffer_percent = align::DEFAULT_BUFFER_PERCENT,
                                                       bool normalize = false);
    [[nodiscard]] static glm::vec2 calculate_alignment_edge(uint16_t alignment,
                                                            float width,
                                                            float height,
                                                            float buffer_percent = align::DEFAULT_BUFFER_PERCENT,
                                                            bool normalize = false);
    [[nodiscard]] static float calculate_alignment_edge_hz(uint16_t alignment,
                                                           float width,
                                                           float buffer_percent = align::DEFAULT_BUFFER_PERCENT,
                                                           bool normalize = false);
    [[nodiscard]] static float calculate_alignment_edge_vt(uint16_t alignment,
                                                           float height,
                                                           float buffer_percent = align::DEFAULT_BUFFER_PERCENT,
                                                           bool normalize = false);
    [[nodiscard]] static glm::vec2 calculate_alignment_origin(uint16_t alignment, bool normalize = true);
    [[nodiscard]] static float calculate_alignment_origin_hz(uint16_t alignment, bool normalize = true);
    [[nodiscard]] static float calculate_alignment_origin_vt(uint16_t alignment, bool normalize = true);

    static ref_t<ui_overlay_t> get();

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
    std::unordered_set<ref_t<clickable_t>> clickables_{};
    ui_update_fn update_;

    mouse_button_down_fn click_thru_;

    static ref_t<ui_overlay_t> instance_;
    static bool init_;


    std::function<void(int, int)> resize_;
    friend class canvas_t;

};

}

#endif //MB2DC_OVERLAY_H
