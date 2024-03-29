//
// Created by Garrett on 10/30/2021.
//

#ifndef MB2DC_SHADER_HPP
#define MB2DC_SHADER_HPP

#include "gl_util.hpp"
#include "texture.hpp"
#include "core/util.hpp"

#include <glm/glm.hpp>

#include <string>

namespace mb2dc {

class gl_shader_t : public gl_object_t {
public:
    gl_shader_t(const std::string &name, const std::string &vertex_src, const std::string &fragment_src);
    ~gl_shader_t() override;

    void bind() override;
    void unbind() const override;

    /*
     * Pass some values to the vertex and fragment shaders of this shader program as uniforms.
     */
    void set_uniform_int(const std::string &name, int val);
    void set_uniform_int_arr(const std::string &name, int *vals, uint32_t cnt);
    void set_uniform_float(const std::string &name, float val);
    void set_uniform_float_arr(const std::string &name, float *vals, uint32_t cnt);
    void set_uniform_float2(const std::string &name, const glm::vec2 &v);
    void set_uniform_float3(const std::string &name, const glm::vec3 &v);
    void set_uniform_float4(const std::string &name, const glm::vec4 &v);
    void set_uniform_mat3(const std::string &name, const glm::mat3 &m);
    void set_uniform_mat4(const std::string &name, const glm::mat4 &m);

    void enable_texture(const std::string &name, const ref_t<gl_texture_t> &texture);

    const std::string &name() { return this->name_; }

    static const std::string DEFAULT_NAME,
                             DEFAULT_VERT_PATH,
                             DEFAULT_FRAG_PATH;

    static const std::string defaults[3];

    static ref_t<gl_shader_t> get_default();

private:
    /*
     * Reads two glsl shaders and compiles them into a shader program.
     * Throws a gl_load_ex exception on failure.
     */
    void read_glsl_files(const std::string &vertex_src_path, const std::string &fragment_src_path);

    /*
     * Compiles a shader of 'type' with 'src' and returns the gl id for said shader.
     */
    static uint32_t compile(uint32_t type, const std::string &src);

    /*
     * name_: a name for the shader program.
     */
    std::string name_;

    static ref_t<gl_shader_t> default_instance_;
};

}

#endif
