//
// Created by Garrett on 11/18/2022.
//

#ifndef MB2DC_FONT_MANAGER_H
#define MB2DC_FONT_MANAGER_H

#include "core/util.hpp"
#include "graphics/shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

namespace mb2dc {

class font_t;

class font_manager_t {
public:
    ~font_manager_t();

    void load(const ref_t<font_t>& font);
    ref_t<font_t> load(const std::string &font_path, unsigned int height = 72);

    void unload(const ref_t<font_t>& font);

    std::unordered_map<std::string, ref_t<font_t>> loaded() const { return this->font_cache_; }

    static font_manager_t *get();
    static ref_t<gl_shader_t> text_shader_;

private:
    font_manager_t();
    std::unordered_map<std::string, ref_t<font_t>> font_cache_;

    static font_manager_t *instance_;
    static FT_Library g_library_;

    friend font_t;
};

}

#endif //MB2DC_FONT_MANAGER_H
