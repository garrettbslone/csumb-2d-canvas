//
// Created by Garrett on 11/18/2022.
//

#include <core/exception.hpp>

#include <ui/glyph.hpp>
#include <ui/font.hpp>

#include <filesystem>

namespace mb2dc {

ref_t<font_t> font_t::arial_ = nullptr;
ref_t<font_t> font_t::inkfree_ = nullptr;

font_t::font_t(const std::string &font_path, uint32_t width, uint32_t height)
{
    // find path to font
    if (font_path.empty()) {
        throw font_load_ex("Failed to load font: " + std::string(font_path));
    }

    // load font as face
    if (FT_New_Face(font_manager_t::g_library_, font_path.c_str(), 0, &this->face_)) {
        throw font_load_ex("Failed to load font: " + std::string(font_path));
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(this->face_, width, height);

    this->name_ = name_from_path(font_path);
    this->preload_printable_ascii();
}

font_t::~font_t()
{
    for (auto &c: this->glyph_cache_) {
        delete c;
    }

    FT_Done_Face(this->face_);
}

glyph_t font_t::load(char c, glm::vec2 pos, float scale)
{
    if (!std::isprint(c)) {

    }

    auto i = ascii_cast(c - CHAR_OFFSET);
    auto g = this->glyph_cache_[i];
    if (g != nullptr) {
        return *g;
    }

    auto glyph = new glyph_t(c, this, pos, scale);

    if (!glyph) {
        throw font_load_ex("failed to load glyph " + std::string(&c) + " from font " + this->name_);
    }

    this->glyph_cache_[i] = glyph;
    return *glyph;
}

void font_t::load(glyph_t *glyph)
{
    if (!glyph) {
        return;
    }

    auto i = ascii_cast(glyph->character());
    if (this->glyph_cache_[i] == nullptr) {
        this->glyph_cache_[i] = glyph;
    }
}

void font_t::unload(char c)
{
    auto i = ascii_cast(c - CHAR_OFFSET);
    auto glyph = this->glyph_cache_[i];

    if (glyph != nullptr) {
        delete glyph;
        this->glyph_cache_[i] = nullptr;
    }
}

std::string font_t::name_from_path(std::string_view path)
{
    auto _p = std::filesystem::path(path);

    return _p.filename().string();
}

void font_t::load_defaults()
{
    auto manager = font_manager_t::get();

    arial_ = manager->load(get_res("fonts/arial.ttf"));
    inkfree_ = manager->load(get_res("fonts/Inkfree.ttf"));
}

void font_t::preload_printable_ascii()
{
    this->glyph_cache_ = {};
    for (char c = CHAR_OFFSET; c < (char) (static_cast<uint8_t>(CHAR_OFFSET) + NUM_PRINTABLE_ASCII_CHARS); c++) {
        static_cast<void>(this->load(c, {0.f, 0.f}, 1.f));
    }
}

}