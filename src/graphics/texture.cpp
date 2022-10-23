//
// Created by Garrett on 10/29/2021.
//

#include <core/exception.hpp>
#include <graphics/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

namespace mb2dc {

uint32_t gl_texture_t::active_slots_ = 0;

gl_texture_t::gl_texture_t(uint32_t width, uint32_t height)
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

gl_texture_t::gl_texture_t(const std::string &path)
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

void gl_texture_t::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);
}

void gl_texture_t::unbind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void gl_texture_t::bind()
{
    if (this->slot_ >= MAX_TEXTURE_SLOTS) {
        int slot = find_next_slot();

        if (slot < 0) {
            throw texture_ex("Cannot bind to slot " + std::to_string(slot) +
                            "! Must be between 0 and " + std::to_string(MAX_TEXTURE_SLOTS));
        }

        this->slot_ = slot;
    }

    glActiveTexture(GL_TEXTURE0 + this->slot_);
    glBindTexture(GL_TEXTURE_2D, this->gl_id_);
}

void gl_texture_t::unbind() const
{
    if (this->slot_ < MAX_TEXTURE_SLOTS) {
        glActiveTexture(GL_TEXTURE0 + this->slot_);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

bool gl_texture_t::is_slot_active(uint32_t slot)
{
    // there are only 32 possible texture slots [0, 31]
    if (slot >= MAX_TEXTURE_SLOTS) {
        return false;
    }

    return ((active_slots_ >> slot) & 1) == 1;
}

bool gl_texture_t::set_slot_active(uint32_t slot)
{
    // slot is already in use
    if (((active_slots_ >> slot) & 1) == 1) {
        return false;
    }

    active_slots_ |= 1 << slot;
    return true;
}

int gl_texture_t::find_next_slot()
{
    for (int i = 0; i < MAX_TEXTURE_SLOTS; i++) {
        if (!is_slot_active(i)) {
            set_slot_active(i);
            return i;
        }
    }

    return -1;
}

bool gl_texture_t::operator==(const gl_texture_t &other) const
{
    return this->gl_id_ == ((gl_texture_t &) other).gl_id_;
}

}
