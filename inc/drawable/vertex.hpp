//
// Created by Garrett on 8/4/22.
//

#ifndef MB2DC_VERTEX_HPP
#define MB2DC_VERTEX_HPP

#include <glm/glm.hpp>

#include <vector>

namespace mb2dc {

struct vertex_t {
  glm::vec2 position_{};
  glm::vec4 color_{};
  glm::vec2 uv_{};

  vertex_t(const glm::vec2 &position = glm::vec2(0.f),
           const glm::vec4 &color = glm::vec4(0.f),
           const glm::vec2 &uv = glm::vec2(0.f))
          : position_(position), color_(color), uv_(uv)
  {
  }

  inline bool operator==(const vertex_t &other) const
  {
      return this->position_ == other.position_ &&
             this->color_ == other.color_ &&
             this->uv_ == other.uv_;
  }

  inline bool operator!=(const vertex_t &other)
  {
      return this->position_ != other.position_ ||
              this->color_ != other.color_ ||
              this->uv_ != other.uv_;
  }

  static inline std::vector<vertex_t> from_floats(const std::vector<float> &data)
  {
      assert(data.size() % 8 == 0 && "Data must contain position, color, normal, and uv values for each vertex!");

      std::vector<vertex_t> vertices;
      vertices.reserve(data.size() / 8);

      for (size_t i = 0; i <= data.size() - 8; i += 8) {
          vertices.push_back({
                  glm::vec2{data[i], data[i + 1]},
                  glm::vec4{data[i + 2], data[i + 3], data[i + 4], data[i + 5]},
                  glm::vec2{data[i + 6], data[i + 7]}
          });
      }

      return vertices;
  }

  static inline std::vector<vertex_t> from_floats(
          const std::vector<float> &positions,
          const std::vector<float> &colors,
          const std::vector<float> &uvs
  )
  {
      assert((positions.size() / 2 == colors.size() / 4) == uvs.size() / 2
            && "There must be the same number of vertices for each type of data!");

      std::vector<vertex_t> vertices;
      vertices.reserve(uvs.size() / 2);

      for (size_t i = 0; i <= positions.size() - 2; i += 2) {
          vertices.push_back({
                  glm::vec2{positions[i], positions[i + 1]},
                  glm::vec4{colors[i], colors[i + 1], colors[i + 2], colors[i + 3]},
                  glm::vec2{uvs[i], uvs[i + 1]}
          });
      }

      return vertices;
  }

  static inline std::vector<float> to_floats(const std::vector<vertex_t> &vertices)
  {
      std::vector<float> data;
      data.reserve(vertices.size() * 8);

      for (auto &v: vertices) {
          data.push_back(v.position_.x);
          data.push_back(v.position_.y);

          data.push_back(v.color_.r);
          data.push_back(v.color_.g);
          data.push_back(v.color_.b);
          data.push_back(v.color_.a);

          data.push_back(v.uv_.x);
          data.push_back(v.uv_.y);
      }

      return data;
  }
};

}

#endif //MB2DC_VERTEX_HPP
