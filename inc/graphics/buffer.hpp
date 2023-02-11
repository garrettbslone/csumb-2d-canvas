//
// Created by Garrett on 10/27/2021.
//

#ifndef MB2DC_BUFFER_HPP
#define MB2DC_BUFFER_HPP

#include "gl_util.hpp"
#include "drawable/vertex.hpp"

namespace mb2dc {

enum class buffer_usage_type {
    STATIC_DRAW,
    DYNAMIC_DRAW,
    STREAM_DRAW,
    STATIC_READ,
    DYNAMIC_READ,
    STREAM_READ,
    STATIC_COPY,
    DYNAMIC_COPY,
    STREAM_COPY,
};

#define STATIC_DRAW buffer_usage_type::STATIC_DRAW
#define DYNAMIC_DRAW buffer_usage_type::DYNAMIC_DRAW
#define STREAM_DRAW buffer_usage_type::STREAM_DRAW
#define STATIC_READ buffer_usage_type::STATIC_READ
#define DYNAMIC_READ buffer_usage_type::DYNAMIC_READ
#define STREAM_READ buffer_usage_type::STREAM_READ
#define STATIC_COPY buffer_usage_type::STATIC_COPY
#define DYNAMIC_COPY buffer_usage_type::DYNAMIC_COPY
#define STREAM_COPY buffer_usage_type::STREAM_COPY

/*
 * An OpenGL defined vertex_buffer.
 */
class gl_vertex_buffer_t : public gl_object_t {
public:
    explicit gl_vertex_buffer_t(const std::vector<vertex_t> &vertices, buffer_usage_type usage = STATIC_DRAW);
    explicit gl_vertex_buffer_t(const std::vector<float> &vertices,
                                const std::vector<uint8_t> &attributes,
                                buffer_usage_type usage = DYNAMIC_DRAW);
    ~gl_vertex_buffer_t() override;

    void bind() override;
    void unbind() const override;

    inline std::vector<float> &get_data() { return this->vertices_; }
    [[nodiscard]] unsigned int get_data_size() const;

    void write_data(void *vertices, uint64_t size, uint64_t offset);

    std::vector<uint8_t> attributes_;

private:
    std::vector<float> vertices_;
    buffer_usage_type usage_;
};

/*
 * An OpenGL defined index_buffer.
 */
class gl_index_buffer_t : public gl_object_t {
public:
    explicit gl_index_buffer_t(const std::vector<uint32_t> &indices, buffer_usage_type usage = STATIC_DRAW);
    ~gl_index_buffer_t() override;

    void bind() override;
    void unbind() const override;

    void write_data(void *indices, uint64_t size, uint64_t offset);

    [[nodiscard]] inline uint32_t get_count() const { return this->count_; }

private:
    uint32_t count_;
    std::vector<uint32_t> indices_;
    buffer_usage_type usage_;
};

}

#endif
