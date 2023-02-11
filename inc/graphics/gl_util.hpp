//
// Created by Garrett on 12/2/21.
//

#ifndef MB2DC_GL_UTIL_HPP
#define MB2DC_GL_UTIL_HPP

#include <cstdint>

namespace mb2dc {

class gl_object_t {
public:
    virtual ~gl_object_t() = default;

    virtual void bind() = 0;
    virtual void unbind() const = 0;

    [[nodiscard]] virtual uint32_t id() const final { return this->gl_id_; }

protected:
    uint32_t gl_id_;
};

/*
 * Primitive topology types for drawing geometry.
 */
enum class topology {
  POINTS,
  LINES,
  LINE_LOOP,
  LINE_STRIP,
  TRIANGLES,
  TRIANGLE_STRIP,
  TRIANGLE_FAN,
};

#define POINTS topology::POINTS
#define LINES topology::LINES
#define LINE_LOOP topology::LINE_LOOP
#define LINE_STRIP topology::LINE_STRIP
#define TRIANGLES topology::TRIANGLES
#define TRIANGLE_STRIP topology::TRIANGLE_STRIP
#define TRIANGLE_FAN topology::TRIANGLE_FAN

uint32_t get_topology(topology t);

}

#endif
