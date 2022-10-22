//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_GL_BUFFER_HPP
#define GLW_GL_BUFFER_HPP

#include "gl_util.hpp"
#include "drawable/vertex.hpp"

namespace mb2dc {

/*
 * An OpenGL defined vertex_buffer.
 */
class gl_vertex_buffer_t : public gl_object_t {
public:
    explicit gl_vertex_buffer_t(const std::vector<vertex_t> &vertices);
    ~gl_vertex_buffer_t() override;

    void bind() const override;
    void unbind() const override;

    inline std::vector<vertex_t> &get_data() { return this->vertices_; }
    inline unsigned int get_data_size() const { return this->vertices_.size(); }

    void write_data(void *vertices, uint64_t size, uint64_t offset);

private:
    std::vector<vertex_t> vertices_;
};

/*
 * An OpenGL defined index_buffer.
 */
class gl_index_buffer_t : public gl_object_t {
public:
    explicit gl_index_buffer_t(const std::vector<uint32_t> &indices);
    ~gl_index_buffer_t() override;

    void bind() const override;
    void unbind() const override;

    void write_data(void *indices, uint64_t size, uint64_t offset);

    inline uint32_t get_count() const { return this->count_; }

private:
    uint32_t count_;
    std::vector<uint32_t> indices_;
};

}

#endif //GLW_GL_BUFFER_HPP
