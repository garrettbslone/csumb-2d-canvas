//
// Created by Garrett on 10/29/2021.
//

#include <core/exception.hpp>
#include <graphics/texture.hpp>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

namespace mb2dc {

gl_texture_params gl_texture_params::img_default = gl_texture_params(RGBA, REPEAT, LINEAR);
gl_texture_params gl_texture_params::text_default = gl_texture_params(RED, CLAMP_TO_EDGE, LINEAR);

int gl_texture_params::get_gl_fmt(fmt f)
{
    switch (f) {
        case RGB:
            return GL_RGB;
        case RGBA:
            return GL_RGBA;
        case RED:
            return GL_RED;
        case BLUE:
            return GL_BLUE;
        case GREEN:
            return GL_GREEN;
        case ALPHA:
            return GL_ALPHA;
        default:
            return GL_RGBA;
    }
}

int gl_texture_params::get_gl_wrap(gl_texture_params::wrap w)
{
    switch (w) {
        case REPEAT:
            return GL_REPEAT;
        case MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        case CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_REPEAT;
    }
}

int gl_texture_params::get_gl_scale_filter(gl_texture_params::scale_filter sf)
{
    switch (sf) {
        case LINEAR:
            return GL_LINEAR;
        case NEAREST:
            return GL_NEAREST;
        case NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        case NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        default:
            return GL_LINEAR;
    }
}

gl_texture_t::gl_texture_t(uint32_t width, uint32_t height, const unsigned char *img_data, gl_texture_params params)
{
    this->width_ = width;
    this->height_ = height;
    this->loaded_ = false;

    this->internal_fmt_ = gl_texture_params::get_gl_fmt(params.fmt_);
    this->data_fmt_ = this->internal_fmt_;

    glGenTextures(1, &this->gl_id_);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);

    int w = gl_texture_params::get_gl_wrap(params.wrap_),
        _min = gl_texture_params::get_gl_scale_filter(params.min_filter_),
        _max = gl_texture_params::get_gl_scale_filter(params.max_filter_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _max);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, w);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, w);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 this->data_fmt_,
                 this->width_,
                 this->height_,
                 0,
                 this->data_fmt_,
                 GL_UNSIGNED_BYTE,
                 img_data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

gl_texture_t::gl_texture_t(const std::string &path, gl_texture_params params)
{
    this->width_ = -1;
    this->height_ = -1;

    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data) {
        throw gl_load_ex("failed to load texture at: " + path + "\n" + stbi_failure_reason());
    }

    this->loaded_ = true;
    this->width_ = w;
    this->height_ = h;

    if (channels == 4) {
        this->internal_fmt_ = GL_RGBA8;
        this->data_fmt_ = GL_RGBA;
    } else {
        this->internal_fmt_ = GL_RGB8;
        this->data_fmt_ = GL_RGB;
    }

    glGenTextures(1, &this->gl_id_);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);

    w = gl_texture_params::get_gl_wrap(params.wrap_);
    int _min = gl_texture_params::get_gl_scale_filter(params.min_filter_),
            _max = gl_texture_params::get_gl_scale_filter(params.max_filter_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _max);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, w);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, w);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 this->data_fmt_,
                 this->width_,
                 this->height_,
                 0,
                 this->data_fmt_,
                 GL_UNSIGNED_BYTE,
                 data);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

gl_texture_t::~gl_texture_t()
{
    glDeleteTextures(1, &this->gl_id_);
}

void gl_texture_t::set_data(void *data, uint32_t size)
{
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 this->internal_fmt_,
                 this->width_,
                 this->height_,
                 0,
                 this->internal_fmt_,
                 GL_UNSIGNED_BYTE,
                 data);
}

void gl_texture_t::bind(uint8_t slot) const
{

    if (slot >= MAX_TEXTURE_SLOTS || slot < 0) {
        throw texture_ex("Cannot bind to slot " + std::to_string(slot) +
                         "! Must be between 0 and " + std::to_string(MAX_TEXTURE_SLOTS));
    }

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);
}

void gl_texture_t::unbind(uint8_t slot) const
{
    if (this->slot_ < MAX_TEXTURE_SLOTS) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void gl_texture_t::bind()
{
    this->bind(this->slot_);
}

void gl_texture_t::unbind() const
{
    this->unbind(this->slot_);
}

bool gl_texture_t::operator==(const gl_texture_t &other) const
{
    return this->gl_id_ == ((gl_texture_t &) other).gl_id_;
}

}
