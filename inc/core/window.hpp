//
// Created by Garrett on 9/28/2021.
//

#ifndef MB2DC_WINDOW_HPP
#define MB2DC_WINDOW_HPP

#include "input.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace mb2dc {

class window;
class input;

struct window_spec {
    std::string title_{"glw app"};
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
    ref<window *> window_;
    ref<input> input_;
    close_fn close_;
    resize_fn resize_;
};

/*
 * An object that encapsulates a window to be displayed on the screen as well
 * as a graphics api context to render to.
 */
class window {
public:
    explicit window(const window_spec &spec);
    window(const window_spec &spec, const window_data &data);
    ~window();

    /*
     * Avoid dangling pointer's by making windows non copyable.
     */
    window(const window &) = delete;
    window &operator=(const window &) = delete;

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
    void set_close_cb(close_fn cb);
    void set_resize_cb(resize_fn cb);
    /*
     * Set the window's title.
     */
    void set_title(const std::string& title);

    /*
     * Update the window. This should be called once per render pass.
     * Returns true if the window is closing or false otherwise.
     */
    bool update();

    /*
     * Returns a void pointer to the underlying native (OS) window
     * implementation.
     */
    void *get_native_window();

    inline double get_aspect_ratio() { return static_cast<double>(this->spec_.width_) / this->spec_.height_; }

    inline uint32_t get_width() const { return this->spec_.width_; }
    inline uint32_t get_height() const { return this->spec_.height_; }

    bool fb_resized_{false};
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

    ref<GLFWwindow *> native_window_;
    ref<GLFWmonitor *> monitor_;
};

}

#endif //MB2DC_WINDOW_HPP
