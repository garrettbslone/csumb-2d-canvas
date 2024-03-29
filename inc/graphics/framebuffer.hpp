//
// Created by Garrett on 10/27/2021.
//

#ifndef MB2DC_FRAMEBUFFER_HPP
#define MB2DC_FRAMEBUFFER_HPP

#include "gl_util.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>

namespace mb2dc {

using framebuffer_resize_fn = std::function<void(int, int)>;

class gl_framebuffer_t : public gl_object_t {
public:
    gl_framebuffer_t();
    ~gl_framebuffer_t() override;

    void clear();
    void destroy();
    void reset();

    void bind() override;
    void unbind() const override;

    void resize(uint32_t width, uint32_t height) const;

    [[nodiscard]] constexpr inline uint32_t get_width() const { return this->width_; }
    [[nodiscard]] constexpr inline uint32_t get_height() const { return this->height_; }

    framebuffer_resize_fn resize_cb_;
    glm::vec4 clear_clr_;

private:
    uint32_t width_, height_;
};

}

#endif
