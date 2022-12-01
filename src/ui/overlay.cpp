//
// Created by Garrett on 11/18/2022.
//

#include <ui/overlay.hpp>

namespace mb2dc {

ref<ui_overlay_t> ui_overlay_t::instance_ = nullptr;
bool ui_overlay_t::init_ = false;

void ui_overlay_t::update(const glm::mat4 &view_proj)
{
    if (this->update_) {
        this->update_(this->queue_.get(), view_proj);
    }
}

ref<text_t> ui_overlay_t::draw_text(std::string_view text, const ref<font_t> &font, glm::vec2 pos, float scale)
{
    auto t = new_ref<text_t>(text, font ? font : font_t::inkfree(), pos, scale);
    this->queue_.enqueue(t.get());
    return t;
}

void ui_overlay_t::draw_text(const ref<text_t> &text)
{
    this->queue_.enqueue(text.get());
}

void ui_overlay_t::draw_element(const ref<ui_element_t> &element)
{
    this->queue_.enqueue(element.get());
}

void ui_overlay_t::erase(ui_element_t *element)
{
    this->queue_.dequeue(element);
}

void ui_overlay_t::erase(const ref<ui_element_t> &element)
{
    this->queue_.dequeue(element.get());
}

ref<ui_overlay_t> ui_overlay_t::get()
{
    if (!instance_) {
        ui_overlay_t o{};
        instance_ = new_ref<ui_overlay_t>(o);
    }

    return instance_;
}

ui_overlay_t::ui_overlay_t()
{
    this->font_manager_ = font_manager_t::get();
    this->update_ = [&] (const std::vector<ui_element_t *> &elements, const glm::mat4 &view_proj)
    {
        for (auto &e: elements) {
            e->draw(view_proj);
        }
    };

    font_t::load_defaults();
    init_ = true;
}

}