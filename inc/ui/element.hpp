//
// Created by Garrett on 11/12/22.
//

#ifndef MB2DC_ELEMENT_HPP
#define MB2DC_ELEMENT_HPP

#include "drawable/drawable.hpp"

#include <functional>

namespace mb2dc {

class ui_element_t : public drawable_t {
public:
    ~ui_element_t() override;
};

class clickable_t;

using ui_click_fn = std::function<void(int btn, clickable_t *thiz)>;

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
