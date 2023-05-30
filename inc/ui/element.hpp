//
// Created by Garrett on 11/12/22.
//

#ifndef MB2DC_ELEMENT_HPP
#define MB2DC_ELEMENT_HPP

#include "drawable/drawable.hpp"

#include <functional>
#include <type_traits>

namespace mb2dc {

namespace align {
    constexpr int16_t NONE = 0;
    constexpr int16_t HZ_LEFT = 1 << 0;
    constexpr int16_t HZ_CENTER = 1 << 1;
    constexpr int16_t HZ_RIGHT = 1 << 2;
    constexpr int16_t VT_TOP = 1 << 3;
    constexpr int16_t VT_CENTER = 1 << 4;
    constexpr int16_t VT_BOTTOM = 1 << 5;
    constexpr int16_t PARENT = 1 << 6;
    constexpr int16_t ANCHOR_EDGE_HZ = 1 << 7;
    constexpr int16_t ANCHOR_EDGE_VT = 1 << 8;
    constexpr int16_t ANCHOR_EDGE = ANCHOR_EDGE_HZ | ANCHOR_EDGE_VT;
    constexpr int16_t ANCHOR_ORIGIN_HZ = 1 << 9;
    constexpr int16_t ANCHOR_ORIGIN_VT = 1 << 10;
    constexpr int16_t ANCHOR_ORIGIN = ANCHOR_ORIGIN_HZ | ANCHOR_ORIGIN_VT;

    constexpr float DEFAULT_BUFFER_PERCENT = 0.005f;
}

class ui_element_t : public drawable_t {
public:
    inline glm::vec2 end() const { return {this->width(), this->height()}; }
    virtual float width() const = 0;
    virtual float height() const = 0;

    inline virtual void align() { this->align(this->alignment_); }
    virtual void align(uint16_t alignment) = 0;

    template<typename T, class... Args>
    [[nodiscard]] ref_t<T> make_child(Args &&... args)
    {
        if constexpr (!std::is_base_of<ui_element_t, T>()) return nullptr;

        auto child = new_ref<T>(args ...);
        child->parent_ = std::dynamic_pointer_cast<ui_element_t>(shared_from_this());

        return child;
    }

    uint16_t alignment_{};

    ref_t<ui_element_t> parent_ = nullptr;
};

class clickable_t;

using ui_click_fn = std::function<void(int btn, const ref_t<clickable_t> &thiz)>;

class clickable_t {
public:
    clickable_t();

    inline void on_click(const ui_click_fn &cb) { this->click_ = cb; }
    [[nodiscard]] inline int id() const { return this->id_; }

    virtual bool overlapping(double x, double y) = 0;
    
    static constexpr uint16_t STATE_CLICKED = 1 << 5;
    static constexpr uint16_t STATE_UNCLICKED = 1 << 6;

protected:
    ui_click_fn  click_;
    int id_;

    static int ID;

    friend class ui_overlay_t;
};

}

#endif //MB2DC_ELEMENT_HPP
