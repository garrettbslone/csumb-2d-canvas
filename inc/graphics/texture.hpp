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

class gl_texture_t : public gl_object_t {
public:
    gl_texture_t(uint32_t width, uint32_t height, const float *img_data);
    explicit gl_texture_t(const std::string &path);
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
