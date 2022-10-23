//
// Created by Garrett on 10/29/2021.
//

#ifndef GLW_GL_TEXTURE_HPP
#define GLW_GL_TEXTURE_HPP

#include "gl_util.hpp"

#include <string>

#define MAX_TEXTURE_SLOTS 32

namespace mb2dc {

class gl_texture_t : public gl_object_t {
public:
    gl_texture_t(uint32_t width, uint32_t height);
    explicit gl_texture_t(const std::string &path);
    ~gl_texture_t() override;

    void set_data(void *data, uint32_t size);

    inline uint32_t get_width() const { return this->width_; }
    inline uint32_t get_height() const { return this->height_; }
    inline uint32_t slot() const { return this->slot_; }
    inline bool is_loaded() const { return this->loaded_; }

    void bind(uint32_t slot) const;
    void unbind(uint32_t slot = 0) const;

    void bind() override;
    void unbind() const override;

    static bool is_slot_active(uint32_t slot);
    static bool set_slot_active(uint32_t slot);
    static int find_next_slot();
    static inline uint32_t get_active_slots()  { return active_slots_; }

    bool operator==(const gl_texture_t &other) const;

private:
    /*
     * internal_format: the number of color components in the texture
     *                  (usually RGBA8 or RGB8).
     * data_format: the format of the pixel data (usually RGBA or RGB).
     */
    uint32_t internal_fmt_, data_fmt_;
    /*
     * There are 32 texture slots in the gl state machine, so we can use 32 bit
     * int as a bit map to keep track of active slots.
     */
    uint32_t width_, height_, slot_{MAX_TEXTURE_SLOTS};
    bool loaded_;

    static uint32_t active_slots_;

};

}

#endif //GLW_GL_TEXTURE_HPP
