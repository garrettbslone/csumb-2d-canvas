//
// Created by Garrett on 11/19/2022.
//

#ifndef MB2DC_TEXT_HPP
#define MB2DC_TEXT_HPP

#include "element.hpp"
#include "glyph.hpp"

namespace mb2dc {

class text_t : public ui_element_t {
public:
    text_t(std::string_view text, const ref<font_t>& font, glm::vec2 pos = {0.f, 0.f}, float scale = 1.f);

    void backspace();
    void clear();
    void append(std::string_view text);
    void change(std::string_view text, unsigned int from = 0);
    void resize(float scale);
    void change_font(const ref<font_t> &font);
    void reposition(glm::vec2 pos);

    void generate(std::string_view text, font_t *font, glm::vec2 pos, float scale, unsigned int i = 0);

    float width() const;
    float height() const;
    glm::vec2 end() const;

    inline glm::vec2 anchor() const { return this->anchor_pos_; }
    inline float font_scale() const { return this->scale_; }
    inline unsigned int size() const { return this->end_; }
    inline unsigned int reserved() const { return this->glyphs_.size(); }

    void draw(const glm::mat4 &view_proj) const override;

    // Max length (number of glyphs) of a text_t object
    static constexpr int MAX_LEN = 1024;

private:
    std::vector<glyph_t> glyphs_;

    unsigned int end_{};

    float scale_{};
    glm::vec2 anchor_pos_{};
};

}

#endif //MB2DC_TEXT_HPP
