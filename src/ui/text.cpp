//
// Created by Garrett on 11/19/2022.
//

#include <glad/gl.h>
#include <ui/text.hpp>

namespace mb2dc {

text_t::text_t(std::string_view text, const ref<font_t>& font, glm::vec2 pos, float scale)
{
    this->shader_ = font_manager_t::text_shader_;
    this->va_ = new_ref<gl_vertex_array_t>();
    this->generate(text, font.get(), pos, scale);
    this->set_z_index(100);
    this->name_ = text;
}

void text_t::backspace()
{
    if (this->end_ > 0) {
        this->end_--;
    }
}

void text_t::clear()
{
    this->end_ = 0;
}

void text_t::append(std::string_view text)
{
    this->change(text, this->end_);
}

void text_t::change(std::string_view text, unsigned int from)
{
    if (this->glyphs_.empty()) {
        throw text_ex("Cannot change the text in an uninitialized text_t object!");
    }

    this->generate(text, this->glyphs_[0].font(), this->anchor_pos_, this->scale_, from);
}

void text_t::resize(float scale)
{
    if (this->end_ == 0 || this->glyphs_.empty()) {
        throw text_ex("Cannot resize the text in an uninitialized text_t object!");
    }

    std::string s;

    for (size_t g = 0; g < this->end_; g++) {
        s += this->glyphs_[g].character();
    }

    this->generate(s, this->glyphs_[0].font(), this->anchor_pos_, scale);
}

void text_t::change_font(const ref<font_t> &font)
{
    if (this->end_ == 0 || this->glyphs_.empty()) {
        throw text_ex("Cannot change the font of the text in an uninitialized text_t object!");
    }

    std::string s;

    for (size_t g = 0; g < this->end_; g++) {
        s += this->glyphs_[g].character();
    }

    this->generate(s, font.get(), this->anchor_pos_, this->scale_);
}

void text_t::reposition(glm::vec2 pos)
{
    if (this->end_ == 0 || this->glyphs_.empty()) {
        throw text_ex("Cannot reposition the text in an uninitialized text_t object!");
    }

    std::string s;

    for (size_t g = 0; g < this->end_; g++) {
        s += this->glyphs_[g].character();
    }

    this->generate(s, this->glyphs_[0].font(), pos, this->scale_);
}

void text_t::generate(std::string_view text, font_t *font, glm::vec2 pos, float scale, unsigned int i)
{
    if (i != 0 && (i > this->end_ || i > this->glyphs_.size())) {
        throw text_ex("Index " + std::to_string(i) + " is past the end of the string and not a valid index");
    }

    this->anchor_pos_ = pos;
    this->scale_ = scale;

    if (i != 0) {
        pos.x = this->glyphs_[i - 1].pos_.x + this->glyphs_[i - 1].next_x();
    }

    if (this->glyphs_.size() < i + text.size()) {
        auto _g = font->load('?', pos, scale);
        _g.shader_ = this->shader_;

        this->glyphs_.resize(pow_2_greater(i + text.size()), _g);
    }

    this->end_ = i + text.size();

    for (char c: text) {
        this->glyphs_[i] = font->load(c, pos, scale);
        this->glyphs_[i].modify(pos, scale);
        this->glyphs_[i].shader_ = this->shader_;

        pos.x += this->glyphs_[i++].next_x();
    }
}

float text_t::width() const
{
    float delta = 0;

    if (this->end_ > 0) {
        delta = this->glyphs_[this->end_].next_x() + this->glyphs_[this->end_].pos_.x - this->anchor_pos_.x;
    }

    return delta;
}

float text_t::height() const
{
    return 0;
}

glm::vec2 text_t::end() const
{
    return {this->width(), this->anchor_pos_.y};
}

void text_t::draw(const glm::mat4 &view_proj) const
{
    this->va_->bind();
    this->shader_->bind();
    this->shader_->set_uniform_float("zIndex", this->z_index_);

    for (size_t i = 0; i < this->end_; i++) {
        auto g = this->glyphs_[i];
        this->shader_->enable_texture("uTex", g.textures_.begin()->second);
        this->shader_->set_uniform_mat4("MVP", g.get_model_mat() * view_proj);
        this->va_->set_vertex_buffer(g.vb_);

        glDrawArrays(
                get_topology(g.topo_),
                0,
                (GLint) g.vb_->get_data_size());
    }
}
}