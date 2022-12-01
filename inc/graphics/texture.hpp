//
// Created by Garrett on 10/29/2021.
//

#ifndef GLW_GL_TEXTURE_HPP
#define GLW_GL_TEXTURE_HPP

#include "gl_util.hpp"
#include "core/util.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

// minimum number of textures supported by OpenGL 3.3 is 48
#define MAX_TEXTURE_SLOTS 48

namespace mb2dc {

struct gl_texture_params {
     enum fmt {
        RGB = 0,
        RGBA = 1,
        RED = 2,
        BLUE = 3,
        GREEN = 4,
        ALPHA = 5
    };

    enum wrap {
        REPEAT = 0,
        MIRRORED_REPEAT = 1,
        CLAMP_TO_EDGE = 2,
        CLAMP_TO_BORDER = 3
    };

    enum scale_filter {
        LINEAR = 0,
        NEAREST = 1,
        NEAREST_MIPMAP_NEAREST = 2,
        LINEAR_MIPMAP_NEAREST = 3,
        LINEAR_MIPMAP_LINEAR = 4,
        NEAREST_MIPMAP_LINEAR = 5
    };

    fmt fmt_;
    wrap wrap_;
    scale_filter min_filter_, max_filter_;

    static gl_texture_params text_default;
    static gl_texture_params img_default;

    inline gl_texture_params(fmt f, wrap w, scale_filter sf)
        : fmt_(f), wrap_(w), min_filter_(sf), max_filter_(sf)
    {}

    inline gl_texture_params(fmt f, wrap w, scale_filter _min, scale_filter _max)
            : fmt_(f), wrap_(w), min_filter_(_min), max_filter_(_max)
    {}

    inline gl_texture_params(int f, int w, int sf)
    {
        this->fmt_ = static_cast<fmt>(f);
        this->wrap_ = static_cast<wrap>(w);
        this->min_filter_ = static_cast<scale_filter>(sf);
        this->max_filter_ = this->min_filter_;
    }

    inline gl_texture_params(int f, int w, int _min, int _max)
    {
        this->fmt_ = static_cast<fmt>(f);
        this->wrap_ = static_cast<wrap>(w);
        this->min_filter_ = static_cast<scale_filter>(_min);
        this->max_filter_ = static_cast<scale_filter>(_max);
    }

    static int get_gl_fmt(fmt f);
    static int get_gl_wrap(wrap w);
    static int get_gl_scale_filter(scale_filter sf);
};

class gl_texture_t : public gl_object_t {
public:
    gl_texture_t(uint32_t width, uint32_t height, const unsigned char *img_data,
                 gl_texture_params params = gl_texture_params::text_default);
    explicit gl_texture_t(const std::string &path, gl_texture_params params = gl_texture_params::img_default);
    ~gl_texture_t() override;

    void set_data(void *data, uint32_t size);

    inline uint32_t get_width() const { return this->width_; }
    inline uint32_t get_height() const { return this->height_; }
    inline uint8_t slot() const { return this->slot_; }
    inline bool is_loaded() const { return this->loaded_; }

    void bind(uint8_t slot) const;
    void unbind(uint8_t slot) const;

    void bind() override;
    void unbind() const override;

    bool operator==(const gl_texture_t &other) const;

    float alpha_{-1.f};

private:
    /*
     * internal_format: the number of color components in the texture
     *                  (usually RGBA8 or RGB8).
     * data_format: the format of the pixel data (usually RGBA or RGB).
     */
    int internal_fmt_, data_fmt_;
    int width_, height_;

    uint8_t slot_{0};

    bool loaded_;

};

}

#endif //GLW_GL_TEXTURE_HPP
