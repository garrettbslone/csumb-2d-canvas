//
// Created by Garrett on 11/18/2022.
//

#include <ui/overlay.hpp>

namespace mb2dc {

ref_t<ui_overlay_t> ui_overlay_t::instance_ = nullptr;
bool ui_overlay_t::init_ = false;

void ui_overlay_t::update(const glm::mat4 &view_proj)
{
    if (this->update_) {
        this->update_(this->queue_.get(), view_proj);
    }
}

void ui_overlay_t::try_click(int btn, double x, double y)
{
    for (auto &c: this->clickables_) {
        if (c->overlapping(x, y)) {
            c->click_(btn, c);
            return;
        }
    }

    if (this->click_thru_) {
        this->click_thru_(btn);
    }
}

void ui_overlay_t::click_thru(mouse_button_down_fn cb)
{
    this->click_thru_ = std::move(cb);
}

ref_t<text_t> ui_overlay_t::draw_text(std::string_view text, const ref_t<font_t> &font, glm::vec2 pos, float scale)
{
    auto t = new_ref<text_t>(text, font ? font : font_t::inkfree(), pos, scale);
    this->queue_.enqueue(t.get());
    return t;
}

void ui_overlay_t::draw_text(const ref_t<text_t> &text)
{
    this->queue_.enqueue(text.get());
}

void ui_overlay_t::draw_element(const ref_t<ui_element_t> &element)
{
    this->queue_.enqueue(element.get());

    clickable_t *c = nullptr;
    if ((c = dynamic_cast<clickable_t *>(element.get())) && this->clickables_.count(c) == 0) {
        this->clickables_.insert(c);
    }
}

void ui_overlay_t::erase(ui_element_t *element)
{
    this->queue_.dequeue(element);

    auto *c = dynamic_cast<clickable_t *>(element);
    if (c) {
        auto it = this->clickables_.find(c);

        if (it != this->clickables_.end()) {
            this->clickables_.erase(it);
        }
    }
}

void ui_overlay_t::erase(const ref_t<ui_element_t> &element)
{
    this->erase(element.get());
}

ref_t<ui_overlay_t> ui_overlay_t::get()
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