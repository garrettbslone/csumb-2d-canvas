//
// Created by Garrett on 11/18/2022.
//

#ifndef MB2DC_FONT_H
#define MB2DC_FONT_H

#include "font_manager.hpp"

#include <string>

namespace mb2dc {

class glyph_t;

class font_t {
public:
    explicit font_t(const std::string &font_path, uint32_t width = 0, uint32_t height = 72);

    FT_Face face() const { return this->face_; }
    const std::string &name() const { return this->name_; }

    glyph_t load(char c, glm::vec2 pos, float scale);
    void load(glyph_t *glyph);

    void unload(char c);

    std::unordered_map<char, glyph_t *> loaded() const { return this->glyph_cache_; }

    static std::string name_from_path(std::string_view path);

    static void load_defaults();

    static const ref<font_t> &arial() { return arial_; }
    static const ref<font_t> &inkfree() { return inkfree_; }

private:
    void preload_ascii();

    std::unordered_map<char, glyph_t *> glyph_cache_;

    FT_Face face_;
    std::string name_;

    static ref<font_t> arial_, inkfree_;
};

}

#endif //MB2DC_FONT_H
