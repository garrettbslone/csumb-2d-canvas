//
// Created by Garrett on 11/12/22.
//

#ifndef MB2DC_BUTTON_HPP
#define MB2DC_BUTTON_HPP

#include "element.hpp"

namespace mb2dc {

class button_t : public ui_element_t, public clickable_t {
public:
    explicit button_t(glm::vec4 border_color = {0.2f, 0.2f, 0.2f, 1.f}, float border_radius = 1.f);

    ~button_t() override;

    void draw(const glm::mat4 &view_proj) const override;

    glm::vec2 convert_to_spos(const glm::vec2 &v);

    bool overlapping(double x, double y) override;

    void set_border_radius(float border_radius);
    inline float border_radius() const { return this->border_radius_; }

    inline float width() const override { return 100.f * this->scale_.x; }
    inline float height() const override { return 20.f * this->scale_.y; }

    inline void align(uint16_t alignment) override
    {
        this->alignment_ = alignment;
        this->translate(ui_overlay_t::calculate_alignment(
            this->alignment_,
            this->width(),
            this->height(),
            align::DEFAULT_BUFFER_PERCENT,
            true
        ));
    }

    glm::vec4 border_color_;

private:
    float border_radius_;
};

}

#endif //MB2DC_BUTTON_HPP
