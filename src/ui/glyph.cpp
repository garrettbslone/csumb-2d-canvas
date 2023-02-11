//
// Created by Garrett on 11/18/2022.
//

#include <ui/glyph.hpp>

#include <glad/gl.h>

namespace mb2dc {

glyph_t::glyph_t(char c, font_t *font, glm::vec2 pos, float scale)
    : pos_(pos), scale_(scale), c_(c), font_(font)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (FT_Load_Char(font->face(), c, FT_LOAD_RENDER)) {
        throw font_load_ex("Couldn't create glyph for " + std::string(&c) + " with font: " + font->name());
    }

    this->textures_.emplace("bitmap", new_ref<gl_texture_t>(font->face()->glyph->bitmap.width,
                                                            font->face()->glyph->bitmap.rows,
                                                            font->face()->glyph->bitmap.buffer));

    this->size_ = {font->face()->glyph->bitmap.width, font->face()->glyph->bitmap.rows};
    this->bearing_ = {font->face()->glyph->bitmap_left, font->face()->glyph->bitmap_top};
    this->advance_ = font->face()->glyph->advance.x;

    this->ib_ = nullptr;
    this->vb_ = nullptr;
    this->va_ = nullptr;

    this->shader_ = nullptr;
    this->topo_ = TRIANGLES;
}

void glyph_t::resize(float scale)
{
    this->scale_ = scale;
    this->set_vertices();
}

void glyph_t::move(glm::vec2 pos)
{
    this->pos_ = pos;
    this->set_vertices();
}

void glyph_t::modify(glm::vec2 pos, float scale)
{
    this->pos_ = pos;
    this->scale_ = scale;
    this->set_vertices();
}

float glyph_t::next_x() const
{
    return (this->advance_ >> 6) * this->scale_;
}

void glyph_t::set_vertices()
{
    float xpos = this->pos_.x + this->bearing_.x * this->scale_;
    float ypos = this->pos_.y - (this->size_.y - this->bearing_.y) * this->scale_;

    float w = this->size_.x * this->scale_;
    float h = this->size_.y * this->scale_;

    std::vector<float> vertices = {
        xpos, ypos + h, 0.0f, 0.0f,
        xpos, ypos, 0.0f, 1.0f,
        xpos + w, ypos, 1.0f, 1.0f,
        xpos, ypos + h, 0.0f, 0.0f,
        xpos + w, ypos, 1.0f, 1.0f,
        xpos + w, ypos + h, 1.0f, 0.0f,
    };

    auto it = this->textures_.find("bitmap");

    if (it == this->textures_.end()) {
        throw font_load_ex("Could not load glyph " + std::string(&this->c_) + " with font " +
                           this->font_->name() + "! No Bitmap texture found!");
    }

    if (!this->vb_.get()) {
        this->vb_ = new_ref<gl_vertex_buffer_t>(vertices, std::vector<uint8_t>{4}, STATIC_DRAW);
    } else {
        this->vb_->write_data(reinterpret_cast<void *>(vertices.data()), vertices.size(), 0);
    }
}

} // mb2dc