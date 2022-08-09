//
// Created by Garrett on 10/29/2021.
//

#include <core/exception.hpp>
#include <graphics/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

namespace mb2dc {

gl_texture::gl_texture(uint32_t width, uint32_t height)
{
    this->width_ = width;
    this->height_ = height;
    this->loaded_ = false;

    this->internal_fmt_ = GL_RGBA8;
    this->data_fmt_ = GL_RGBA;

    glGenTextures(1, &this->gl_id_);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

gl_texture::gl_texture(const std::string &path)
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

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
}

gl_texture::~gl_texture()
{
    glDeleteTextures(1, &this->gl_id_);
}

void gl_texture::set_data(void *data, uint32_t size)
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

void gl_texture::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);
}

void gl_texture::unbind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gl_texture::bind() const
{

}

void gl_texture::unbind() const
{

}

bool gl_texture::is_slot_active(uint32_t slot)
{
    // there are only 32 possible texture slots [0, 31]
    if (slot > 31) {
        return false;
    }

    return ((this->active_slots_ >> slot) & 1) == 1;
}

bool gl_texture::set_slot_active(uint32_t slot)
{
    // slot is already in use
    if (((this->active_slots_ >> slot) & 1) == 1) {
        return false;
    }

    this->active_slots_ |= 1 << slot;
    return true;
}

bool gl_texture::operator==(const gl_texture &other) const
{
    return this->gl_id_ == ((gl_texture &) other).gl_id_;
}

}
