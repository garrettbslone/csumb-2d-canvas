//
// Created by Garrett on 12/7/2021.
//

#ifndef MB2DC_CIRCLE_HPP
#define MB2DC_CIRCLE_HPP

#include "drawable.hpp"

namespace mb2dc {

class circle_t : public drawable_t {
public:
    explicit circle_t(int vertex_count, int r = 100);

    circle_t(const std::string &name, int vertex_count, int r = 100);
    circle_t(const glm::vec4 &color, int vertex_count, int r = 100);
    circle_t(const std::vector<glm::vec4> &colors, int vertex_count, int r = 100);
    circle_t(const std::string &name, const glm::vec4 &color, int vertex_count, int r = 100);
    circle_t(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r = 100);

    ~circle_t() override;

private:
    void create(const std::string &name, const std::vector<glm::vec4> &colors, int vertex_count, int r);

    int r_{-1};

    static int created_, alive_;
};

}

#endif
