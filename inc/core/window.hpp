//
// Created by Garrett on 9/28/2021.
//

#ifndef MB2DC_WINDOW_HPP
#define MB2DC_WINDOW_HPP

#include "input.hpp"

#include <graphics/framebuffer.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace mb2dc {

class window_t;
class input_t;

struct window_spec {
    std::string title_{"2D Canvas"};
    uint32_t width_{1280}, height_{720};
    bool fullscreen_{false}, maximized_{false};
    float alpha_{1.f};
};

/*
 * Callback function to be called upon closing the window.
 */
using close_fn = std::function<void(void)>;
using resize_fn = std::function<void(int, int)>;

struct window_data {
    ref_t<window_t *> window_;
    ref_t<input_t> input_;
    close_fn close_;
    resize_fn resize_;
};

/*
 * An object that encapsulates a window to be displayed on the screen as well
 * as a graphics api context to render to.
 */
class window_t {
public:
    explicit window_t(const window_spec &spec);
    window_t(const window_spec &spec, const window_data &data);
    ~window_t();

    /*
     * Avoid dangling pointer's by making windows non copyable.
     */
    window_t(const window_t &) = delete;
    window_t &operator=(const window_t &) = delete;

    /*
     * Clear the screen through the rendering context.
     */
    void clear() const;

    /*
     * Force a window into fullscreen mode. If the window is already in fullscreen,
     * it will stay in fullscreen.
     */
    void force_fullscreen();

    /*
     * Resize the window. If the window is in fullscreen mode, this call has no
     * effect.
     */
    void resize(uint32_t width, uint32_t height);
    void on_resize(resize_fn cb);

    void on_close(close_fn cb);

    /*
     * Set the window's title.
     */
    void set_title(const std::string& title);

    void set_resizable(bool resizeable);

    /*
     * Update the window. This should be called once per render pass.
     * Returns true if the window is closing or false otherwise.
     */
    bool update();

    /*
     * Returns a void pointer to the underlying native (OS) window
     * implementation.
     */
    void *native_window();

    [[nodiscard]] constexpr inline double aspect_ratio() const
    {
        return static_cast<double>(this->spec_.width_) / this->spec_.height_;
    }

    [[nodiscard]] constexpr inline uint32_t width() const { return this->spec_.width_; }
    [[nodiscard]] constexpr inline uint32_t height() const { return this->spec_.height_; }

    bool fb_resized_{false}, event_driven_{false};
    const char *err_{nullptr};
    glm::vec4 clear_clr_;

    window_spec spec_;
    window_data data_;

    void resize_framebuffer();

private:
    /*
     * GLFW framebuffer resize callback signature.
     */
    void resize_framebuffer(GLFWwindow *w, uint32_t width, uint32_t height);
    /*
     * GLFW window close callback signature.
     */
    void window_set_close(GLFWwindow *w) const;
    /*
    * Creates the underlying native (GLFW) window.
    */
    void create();

    ref_t<GLFWwindow *> native_window_;
    ref_t<GLFWmonitor *> monitor_;
    ref_t<gl_framebuffer_t> fb_;
};

}

#endif //MB2DC_WINDOW_HPP
