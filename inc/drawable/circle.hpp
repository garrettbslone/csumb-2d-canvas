//
// Created by Garrett on 12/7/2021.
//

#ifndef WORMY_HPP
#define WORMY_HPP

#include "drawable.hpp"

namespace mb2dc {

class circle : public drawable {
public:
    explicit circle(int vertex_count, int r = 100);

    circle(const std::string &name, int vertex_count, int r = 100);
    circle(const glm::vec4 &color, int vertex_count, int r = 100);
    circle(const std::vector<glm::vec4> &colors, int vertex_count, int r = 100);
    circle(const std::string &name, const glm::vec4 &color, int vertex_count, int r = 100);
    circle(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r = 100);

    ~circle();

private:
    void create(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r);

    static int created_, alive_;
};

}

#endif //WORMY_HPP
