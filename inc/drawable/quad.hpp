//
// Created by Garrett on 12/6/2021.
//

#ifndef GLW_QUAD_HPP
#define GLW_QUAD_HPP

#include "drawable.hpp"

namespace mb2dc {

class quad : public drawable {
public:
    quad();

    explicit quad(const std::string &name);
    explicit quad(const glm::vec4 &color);
    explicit quad(const std::vector<glm::vec4> &colors);

    quad(const std::string &name, const glm::vec4 &color);
    quad(const std::string &name, const std::vector<glm::vec4> &colors);

    ~quad();

private:
    void create(const std::string &name, const std::vector<glm::vec4> &colors);

    static int created_, alive_;
};

}

#endif //GLW_QUAD_HPP
