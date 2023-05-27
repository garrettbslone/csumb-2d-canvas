//
// Created by Garrett on 11/18/2022.
//

#include <core/canvas.hpp>
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
    auto t = new_ref<text_t>(text, font.get() ? font : font_t::inkfree(), pos, scale);
    this->queue_.enqueue(t.get());
    return t;
}

ref_t<text_t> ui_overlay_t::draw_text(std::string_view text, const ref_t<font_t> &font, int16_t alignment, float scale)
{
    auto t = new_ref<text_t>(text, font.get() ? font : font_t::inkfree(), glm::vec2(0.f), scale);
    t->alignment_ = alignment | align::ANCHOR_EDGE;
    t->reposition(calculate_alignment(t->alignment_, t->end().x, t->end().y));

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

/*
 *  -w/2, h/2
 *  ________________
 *  |              |
 *  |              |
 *  |    |--l-|    |
 *  |              |
 *  |______________|
 *                  w/2, -h/2
 */
glm::vec2 ui_overlay_t::calculate_alignment(uint16_t alignment,
                                            float width,
                                            float height,
                                            float buffer_percent,
                                            bool normalize)
{
    if (alignment & align::NONE) {
        return glm::vec2{0.f};
    } else if (alignment & align::ANCHOR_EDGE_HZ && alignment & align::ANCHOR_EDGE_VT) {
        return calculate_alignment_edge(alignment, width, height, buffer_percent, normalize);
    } else if (alignment & align::ANCHOR_ORIGIN_HZ && alignment & align::ANCHOR_ORIGIN_VT) {
        return calculate_alignment_origin(alignment, normalize);
    } else {
        glm::vec2 pos{0.f};

        if (alignment & align::ANCHOR_EDGE_HZ) {
            pos.x = calculate_alignment_edge_hz(alignment, width, buffer_percent, normalize);
        } else if (alignment & align::ANCHOR_ORIGIN_HZ) {
            pos.x = calculate_alignment_origin_hz(alignment, normalize);
        }

        if (alignment & align::ANCHOR_EDGE_VT) {
            pos.y = calculate_alignment_edge_vt(alignment, height, buffer_percent, normalize);
        } else if (alignment & align::ANCHOR_ORIGIN_VT) {
            pos.y = calculate_alignment_origin_vt(alignment, normalize);
        }

        return pos;
    }
}

glm::vec2 ui_overlay_t::calculate_alignment_edge(uint16_t alignment,
                                                 float width,
                                                 float height,
                                                 float buffer_percent,
                                                 bool normalize)
{
    return glm::vec2{
        calculate_alignment_edge_hz(alignment, width, buffer_percent, normalize),
        calculate_alignment_edge_vt(alignment, height, buffer_percent, normalize)
    };
}

float ui_overlay_t::calculate_alignment_edge_hz(uint16_t alignment, float width, float buffer_percent, bool normalize)
{
    using namespace align;

    float pos = 0.f;

    if (alignment & NONE) {
        return pos;
    }

    static auto window = canvas_t::get()->window_;
    auto w = window->width() / 2.f;
    auto w_buf = window->width() * buffer_percent;

    if (normalize) {
        width /= w;
    }

    if ((alignment & HZ_LEFT && alignment & HZ_RIGHT) ||
        (alignment & HZ_CENTER && !(alignment & HZ_LEFT) && !(alignment & HZ_RIGHT))) {
        pos = width / -2.f;
    } else if (alignment & HZ_LEFT && alignment & HZ_CENTER) {
        pos += (-w + w_buf + width / 2.f) / 2.f;
    } else if (alignment & HZ_LEFT) {
        pos += -w + w_buf;
    } else if (alignment & HZ_RIGHT && alignment & HZ_CENTER) {
        pos += (w - width - w_buf + width / 2.f) / 2.f;
    } else if (alignment & HZ_RIGHT) {
        pos += w - width - w_buf;
    }

    if (normalize) {
        pos /= w;
    }

    return pos;
}

float ui_overlay_t::calculate_alignment_edge_vt(uint16_t alignment, float height, float buffer_percent, bool normalize)
{
    using namespace align;

    float pos = 0.f;

    if (alignment & NONE) {
        return pos;
    }

    static auto window = canvas_t::get()->window_;
    auto h = window->height() / 2.f;
    auto h_buf = window->height() * buffer_percent;

    if (normalize) {
        height /= h;
    }

    if ((alignment & VT_BOTTOM && alignment & VT_TOP) ||
        (alignment & VT_CENTER && !(alignment & VT_BOTTOM) && !(alignment & VT_TOP))) {
        pos = height / -4.f;
    } else if (alignment & VT_BOTTOM && alignment & VT_CENTER) {
        pos += (-h + h_buf + height / -4.f) / 2.f;
    } else if (alignment & VT_BOTTOM) {
        pos += -h + h_buf;
    } else if (alignment & VT_TOP && alignment & VT_CENTER) {
        pos += (h - height - h_buf + height / -4.f) / 2.f;
    } else if (alignment & VT_TOP) {
        pos += h - height - h_buf;
    }

    if (normalize) {
        pos /= h;
    }

    return pos;
}

glm::vec2 ui_overlay_t::calculate_alignment_origin(uint16_t alignment, bool normalize)
{
    return glm::vec2{
        calculate_alignment_origin_hz(alignment, normalize),
        calculate_alignment_origin_vt(alignment, normalize)
    };
}

float ui_overlay_t::calculate_alignment_origin_hz(uint16_t alignment, bool normalize)
{
    using namespace align;

    float pos = 0.f;

    if (alignment & NONE) {
        return pos;
    }

    if (alignment & NONE || ((alignment & HZ_LEFT && alignment & HZ_RIGHT) || (alignment & HZ_CENTER &&
        !(alignment & HZ_LEFT) && !(alignment & HZ_RIGHT)))) {
        return pos;
    }

    static auto window = canvas_t::get()->window_;
    auto w = window->width() / 2.f;

    if (alignment & HZ_LEFT && alignment & HZ_CENTER) {
        pos += -w / 2.f;
    } else if (alignment & HZ_LEFT) {
        pos += -w;
    } else if (alignment & HZ_RIGHT && alignment & HZ_CENTER) {
        pos += w / 2.f;
    } else if (alignment & HZ_RIGHT) {
        pos += w;
    }

    if (normalize) {
        pos /= w;
    }

    return pos;
}

float ui_overlay_t::calculate_alignment_origin_vt(uint16_t alignment, bool normalize)
{
    using namespace align;

    float pos = 0.f;

    if (alignment & NONE) {
        return pos;
    }

    if (alignment & NONE || ((alignment & VT_BOTTOM && alignment & VT_TOP) ||
       (alignment & VT_CENTER && !(alignment & VT_BOTTOM) && !(alignment & VT_TOP)))) {
        return pos;
    }

    static auto window = canvas_t::get()->window_;
    auto h = window->height() / 2.f;

    if (alignment & VT_BOTTOM && alignment & VT_CENTER) {
        pos += -h / 2.f;
    } else if (alignment & VT_BOTTOM) {
        pos += -h;
    } else if (alignment & VT_TOP && alignment & VT_CENTER) {
        pos += h / 2.f;
    } else if (alignment & VT_TOP) {
        pos += h;
    }

    if (normalize) {
        pos /= h;
    }

    return pos;
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

    this->resize_ = [&] (int width, int height)
    {
        UNUSED(width)
        UNUSED(height)

        if (!instance_ || !instance_->init_) {
            return;
        }

        for (auto &e: instance_->queue_.get()) {
            if (e && e->alignment_) {
                e->align();
            }
        }
    };

    font_t::load_defaults();
    init_ = true;
}

}