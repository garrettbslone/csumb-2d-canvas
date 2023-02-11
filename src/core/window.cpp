//
// Created by Garrett on 9/28/2021.
//

#include <utility>

#include <core/exception.hpp>
#include <core/window.hpp>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

namespace mb2dc {

window_t::window_t(const window_spec &spec)
    : window_t(spec, {nullptr, nullptr, nullptr, nullptr}) {}

window_t::window_t(const window_spec &spec, const window_data &data)
{
    if (!glfwInit())
        throw viewport_ex("Failed to create window. GLFW init failed");

    this->spec_ = spec;
    this->data_ = data;
    this->monitor_ = new_ref<GLFWmonitor *>(glfwGetPrimaryMonitor());

    if (!this->monitor_ || !(*this->monitor_)) {
        glfwGetError(&this->err_);
        glfwTerminate();

        throw viewport_ex("Failed to find monitor_" + std::string(this->err_));
    }

    const GLFWvidmode *m = glfwGetVideoMode(*this->monitor_);

    if (this->spec_.maximized_) {
        this->spec_.width_ = m->width;
        this->spec_.height_ = m->height;
    }

    this->create();
}

window_t::~window_t()
{
    glfwDestroyWindow(reinterpret_cast<GLFWwindow *>(*this->native_window_));
    glfwTerminate();
}

static void _close_cb()
{
    glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
}

void window_t::clear() const
{
    glClearColor(this->clear_clr_.r, this->clear_clr_.g, this->clear_clr_.b, this->clear_clr_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_t::force_fullscreen()
{
    this->spec_.fullscreen_ = true;
    this->spec_.maximized_ = false;

    if (!this->monitor_ &&
        !(this->monitor_ = new_ref<GLFWmonitor *>(glfwGetPrimaryMonitor()))) {
        glfwGetError(&this->err_);
        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err_));
    }

    const GLFWvidmode *m = glfwGetVideoMode(*this->monitor_);
    if (!m) {
        glfwGetError(&this->err_);

        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err_));
    }

    glfwSetWindowMonitor(*this->native_window_,
                         *this->monitor_,
                         0,
                         0,
                         m->width,
                         m->height,
                         m->refreshRate);

    this->resize_framebuffer();
}

void window_t::resize(uint32_t width, uint32_t height)
{
    this->spec_.width_ = width;
    this->spec_.height_ = height;
    this->spec_.fullscreen_ = false;
    this->spec_.maximized_ = false;

    glfwSetWindowSize(*this->native_window_, width, height);

    this->resize_framebuffer();
}

void window_t::on_resize(resize_fn cb)
{
    this->data_.resize_ = std::move(cb);
}

void window_t::on_close(close_fn cb)
{
    this->data_.close_ = std::move(cb);
}

void window_t::set_title(const std::string& title)
{
    this->spec_.title_ = title;

    glfwSetWindowTitle(*this->native_window_,title.c_str());
}

void window_t::set_resizable(bool resizeable)
{
    glfwSetWindowAttrib(*this->native_window_, GLFW_RESIZABLE, resizeable);
}

bool window_t::update()
{
    glfwMakeContextCurrent(*this->native_window_);

    if (this->event_driven_) {
        glfwWaitEvents();
    } else {
        glfwPollEvents();
    }

    return static_cast<bool>(glfwWindowShouldClose(*this->native_window_));
}

void *window_t::native_window()
{
    return static_cast<void *>(*this->native_window_);
}

void window_t::resize_framebuffer()
{
    int w, h;

    glfwGetFramebufferSize(
            reinterpret_cast<GLFWwindow *>(*this->native_window_),
            &w,
            &h);
    this->fb_->resize(w, h);
}

void window_t::resize_framebuffer(GLFWwindow *w, uint32_t width, uint32_t height)
{
    UNUSED(w)
    this->fb_->resize(width, height);
}

void window_t::window_set_close(GLFWwindow *w) const
{
    UNUSED(w)

    if (this->data_.close_) {
        this->data_.close_();
    }
}

void window_t::create()
{
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!(this->native_window_ = new_ref<GLFWwindow *>(
            glfwCreateWindow(
                this->spec_.width_,
                this->spec_.height_,
                this->spec_.title_.c_str(),
                this->spec_.fullscreen_ ? *this->monitor_ : nullptr,
                nullptr)))) {
        glfwGetError(&this->err_);
        glfwTerminate();

        throw viewport_ex("Failed to create window: " + std::string(this->err_));
    }

    if (!this->data_.close_) {
        this->data_.close_ = _close_cb;
    }

    this->data_.input_ = input_t::get(this->native_window());
    this->data_.resize_ = nullptr;
    this->data_.window_ = new_ref<window_t *>(this);

    auto native_win = *this->native_window_;

    glfwMakeContextCurrent(native_win);
    if (!gladLoadGL(glfwGetProcAddress)) {
        throw gl_load_ex("Failed to initialize OpenGL context. GLAD load failed.");
    }

    glfwSetWindowUserPointer(native_win, &this->data_);
    glfwSwapInterval(0);

    glfwSetWindowCloseCallback(native_win, [](GLFWwindow *w)
    {
        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (data.close_) {
            data.close_();
        }
    });

    glfwSetKeyCallback(native_win, [](GLFWwindow* w, int k, int scancode, int action, int mods)
    {
        UNUSED(scancode)
        UNUSED(mods)

        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (!data.input_) {
            return;
        }

        auto _input = data.input_.get();

        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && _input->key_down_) {
            _input->key_down_(k);
        } else if (action == GLFW_RELEASE && _input->key_up_) {
            _input->key_up_(k);
        }
    });

    glfwSetMouseButtonCallback(native_win, [](GLFWwindow* w, int btn, int action, int mods)
    {
        UNUSED(mods)

        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (!data.input_) {
            return;
        }

        auto _input = data.input_.get();

        if ((action == GLFW_PRESS || action == GLFW_REPEAT) && _input->mouse_btn_down_) {
            _input->mouse_btn_down_(btn);
        } else if (action == GLFW_RELEASE && _input->mouse_btn_up_) {
            _input->mouse_btn_up_(btn);
        }
    });

    glfwSetCursorPosCallback(native_win, [](GLFWwindow *w, double x, double y)
    {
        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (!data.input_) {
            return;
        }

        auto _input = data.input_.get();

        if (_input->mouse_move_) {
            _input->mouse_move_(x, y);
        }
    });

    glfwSetFramebufferSizeCallback(native_win, [](GLFWwindow *w, int width, int height)
    {
        auto data = (window_data *) glfwGetWindowUserPointer(w);

        if (data && data->window_ && (*data->window_)->fb_) {
            (*data->window_)->fb_->resize(width, height);
        }
    });

    glfwSetWindowSizeCallback(native_win, [](GLFWwindow *w, int width, int height)
    {
        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (data.resize_) {
            data.resize_(width, height);
        } else if (data.window_) {
            (*data.window_)->resize(width, height);
        }
    });

    this->fb_ = new_ref<gl_framebuffer_t>();
    this->fb_->clear_clr_ = {1.f, 1.f, 1.f, 1.f};
    this->resize_framebuffer();

    this->clear_clr_ = glm::vec4(0.8f, 0.5f, 0.2f, 1.0f);
}

}
