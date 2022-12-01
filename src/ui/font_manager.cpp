//
// Created by Garrett on 11/18/2022.
//

#include <ui/font.hpp>
#include "ui/font_manager.hpp"


namespace mb2dc {

ref<gl_shader_t> font_manager_t::text_shader_ = nullptr;

font_manager_t *font_manager_t::instance_ = nullptr;
FT_Library font_manager_t::g_library_ = nullptr;

font_manager_t::~font_manager_t()
{
    FT_Done_FreeType(g_library_);
}

void font_manager_t::load(const ref<font_t>& font)
{
    if (this->font_cache_.find(font->name()) != this->font_cache_.end()) {
        this->font_cache_.emplace(font->name(), font);
    }
}

ref<font_t> font_manager_t::load(const std::string &font_path)
{
    auto it = this->font_cache_.find(font_t::name_from_path(font_path));
    if (it != this->font_cache_.end()) {
        return it->second;
    }

    auto font = new_ref<font_t>(font_path);

    if (!font) {
        return nullptr;
    }

    this->font_cache_.emplace(font->name(), font);
    return font;
}

void font_manager_t::unload(const ref<font_t>& font)
{
    auto it = this->font_cache_.find(font->name());

    if (it != this->font_cache_.end()) {
        this->font_cache_.erase(font->name());
    }
}

font_manager_t *font_manager_t::get()
{
    if (!instance_) {
        instance_ = new font_manager_t;
    }

    return instance_;
}

font_manager_t::font_manager_t()
{
    if (!g_library_ && FT_Init_FreeType(&g_library_)) {
            throw font_manager_ex("Failed to load FreeType library!");
    }

    if (!text_shader_) {
        text_shader_ = new_ref<gl_shader_t>("text", get_res("shaders/text.vs.glsl"), get_res("shaders/text.fs.glsl"));
    }
}

}