//
// Created by Garrett on 11/12/22.
//

#ifndef MB2DC_BUTTON_HPP
#define MB2DC_BUTTON_HPP

#include "element.hpp"

#include <functional>

namespace mb2dc {

class button_t : public ui_element_t, public clickable_t {
public:
    explicit button_t(glm::vec4 border_color = {0.2f, 0.2f, 0.2f, 1.f}, float border_radius = 1.f);

    ~button_t() override;

    void draw(const glm::mat4 &view_proj) const override;

    glm::vec2 convert_to_spos(const glm::vec2 &v);

    bool overlapping(double x, double y) override;

    void set_state(uint16_t state, bool clear_previous = false);
    inline uint16_t state() const { return this->state_; }

    void set_border_radius(float border_radius);
    inline float border_radius() const { return this->border_radius_; }

    static constexpr uint16_t STATE_ACTIVE = 1 << 0;
    static constexpr uint16_t STATE_INACTIVE = 1 << 2;
    static constexpr uint16_t STATE_VISIBLE = 1 << 3;
    static constexpr uint16_t STATE_INVISIBLE = 1 << 4;
    static constexpr uint16_t STATE_CLICKED = 1 << 5;
    static constexpr uint16_t STATE_UNCLICKED = 1 << 6;

    glm::vec4 border_color_;

private:
    uint16_t state_;
    float border_radius_;
};

}

#endif //MB2DC_BUTTON_HPP
