//
// Created by Garrett on 8/8/22.
//

#include <graphics/gl_util.hpp>

#include <glad/gl.h>

namespace mb2dc {

uint32_t get_topology(topology t)
{
    switch (t) {
        case POINTS: return GL_POINTS;
        case LINES: return GL_LINES;
        case LINE_LOOP: return GL_LINE_LOOP;
        case LINE_STRIP: return GL_LINE_STRIP;
        case TRIANGLES: return GL_TRIANGLES;
        case TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case TRIANGLE_FAN: return GL_TRIANGLE_FAN;
        default: return GL_TRIANGLES;
    }
}

}