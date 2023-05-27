//
// Created by Garrett on 11/18/2022.
//

#ifndef MB2DC_GLYPH_T_H
#define MB2DC_GLYPH_T_H

#include "graphics/texture.hpp"

#include "element.hpp"
#include "font.hpp"

#include <glm/vec2.hpp>

namespace mb2dc {

class glyph_t : public ui_element_t {
public:
    glyph_t(char c, font_t *font, glm::vec2 pos = {0.f, 0.f}, float scale = 1.f);


    inline float width() const override { return 0.f; }
    inline float height() const override { return 0.f; }

    inline void align(uint16_t alignment) override { UNUSED(alignment) }

    void resize(float scale);
    void move(glm::vec2 pos);
    void modify(glm::vec2 pos, float scale);

    char character() const { return this->c_; }
    font_t *font() const { return this->font_; }

    float next_x() const;

    glm::ivec2 size_, bearing_;  // Offset from baseline to left/top of glyph
    uint32_t advance_;           // Horizontal offset to advance to next glyph

    glm::vec2 pos_;
    float scale_;

private:
    void set_vertices();

    char c_;
    font_t *font_;
};

} // mb2dc

#endif //MB2DC_GLYPH_T_H
