//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_CANVAS_HPP
#define MB2DC_CANVAS_HPP

#include "draw_queue.hpp"
#include "graphics/context.hpp"
#include "ui/overlay.hpp"
#include "window.hpp"

namespace mb2dc {

using update_fn = std::function<void(
    const std::vector<ref_t<drawable_t>> &shapes,
    const glm::mat4 &view_proj)>;

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

    /*
     * Called when the canvas is being updated and contains the canvas's
     * draw queue of drawable_t elements.
     */
    void on_update(update_fn cb);
    void on_window_resize(resize_fn cb);

    /*
     * window wrappers
     */
    void force_fullscreen();
    void on_close(close_fn cb);
    void set_title(const std::string& title);
    void set_clear_color(const glm::vec4 &c);
    double aspect_ratio();
    uint32_t width() const;
    uint32_t height() const;

    /*
     * ui wrappers
     *
     * Called when the ui overlay is being updated and contains the overlay's
     * draw queue of ui_element_t elements.
     */
    void on_ui_update(ui_update_fn cb);
    ref_t<text_t> draw_ui_text(std::string_view text,
                               const ref_t<font_t> &font = nullptr,
                               glm::vec2 pos = {0.f, 0.f},
                               float scale = 1.f);
    ref_t<text_t> draw_ui_text(std::string_view text,
                               const ref_t<font_t> &font = nullptr,
                               int16_t alignment = align::HZ_LEFT | align::VT_TOP,
                               float scale = 1.f);
    void draw_ui_text(const ref_t<text_t> &text);
    void draw_ui_element(const ref_t<ui_element_t> &element);
    void erase_ui_element(const ref_t<ui_element_t> &element);

    /*
     * input wrappers
     */
    glm::vec2 get_mouse_pos_real();
    glm::vec2 get_mouse_pos_rel();
    glm::vec2 get_mouse_pos_rel(uint32_t dim);
    glm::vec2 get_mouse_pos_rel(uint32_t width, uint32_t height);
    void on_mouse_btn_down(mouse_button_down_fn cb);
    void on_mouse_btn_up(mouse_button_up_fn cb);
    void on_mouse_move(mouse_move_fn cb);
    void on_key_down(key_down_fn cb);
    void on_key_up(key_up_fn cb);
    bool get_key(const key_code &k);
    bool get_key_down(const key_code &k);
    bool get_key_up(const key_code &k);

    void register_ui_clicks();

    void fixed_size();
    void resizable();

    void draw_shape(const ref_t<drawable_t>& shape);
    void draw_shape_at(const ref_t<drawable_t>& shape, int x, int y);

    inline static const canvas_t *get() { return canvas_t::instance_; }

    ref_t<window_t> window_;
    ref_t<ui_overlay_t> overlay_;
    glm::mat4 proj_, view_;

private:
    resize_fn resize_;
    update_fn update_;
    ref_t<input_t> input_;
    ref_t<gl_context_t> gl_ctx_;
    bool running_, minimized_;
    draw_queue_t<drawable_t> queue_{};

    static canvas_t *instance_;

    friend drawable_t;
};

}

#endif //MB2DC_CANVAS_HPP
