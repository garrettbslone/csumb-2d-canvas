//
// Created by Garrett on 12/6/2021.
//

#ifndef MB2DC_RECT_HPP
#define MB2DC_RECT_HPP

#include "drawable.hpp"

namespace mb2dc {

class rect_t : public drawable_t {
public:
    rect_t();

    explicit rect_t(const std::string &name);
    explicit rect_t(const glm::vec4 &color);
    explicit rect_t(const std::vector<glm::vec4> &colors);

    rect_t(const std::string &name, const glm::vec4 &color);
    rect_t(const std::string &name, const std::vector<glm::vec4> &colors);

    ~rect_t() override;

private:
    void create(const std::string &name, const std::vector<glm::vec4> &colors);

    static int created_, alive_;
};

}

#endif
