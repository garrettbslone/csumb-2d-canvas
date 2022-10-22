//
// Created by Garrett on 12/6/2021.
//

#ifndef GLW_QUAD_HPP
#define GLW_QUAD_HPP

#include "drawable.hpp"

namespace mb2dc {

class quad_t : public drawable_t {
public:
    quad_t();

    explicit quad_t(const std::string &name);
    explicit quad_t(const glm::vec4 &color);
    explicit quad_t(const std::vector<glm::vec4> &colors);

    quad_t(const std::string &name, const glm::vec4 &color);
    quad_t(const std::string &name, const std::vector<glm::vec4> &colors);

    ~quad_t();

private:
    void create(const std::string &name, const std::vector<glm::vec4> &colors);

    static int created_, alive_;
};

}

#endif //GLW_QUAD_HPP
