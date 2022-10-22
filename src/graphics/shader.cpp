//
// Created by Garrett on 10/30/2021.
//

#include <core/util.hpp>
#include <graphics/shader.hpp>

#include <glad/gl.h>

#include <iostream>

namespace mb2dc {

const std::string gl_shader_t::DEFAULT_NAME = "default",
                  gl_shader_t::DEFAULT_VERT_PATH = std::string() + RES_PATH + "shaders/basic.vs.glsl",
                  gl_shader_t::DEFAULT_FRAG_PATH = std::string() + RES_PATH + "shaders/basic.fs.glsl";

const std::string gl_shader_t::defaults[] = {
    DEFAULT_NAME,
    DEFAULT_VERT_PATH,
    DEFAULT_FRAG_PATH
};

gl_shader_t::gl_shader_t(const std::string &name, const std::string &vertex_src, const std::string &fragment_src)
{
    this->name_ = name;
    this->read_glsl_files(vertex_src, fragment_src);
}

gl_shader_t::~gl_shader_t()
{
    glDeleteProgram(this->gl_id_);
}

void gl_shader_t::bind() const
{
    glUseProgram(this->gl_id_);
}

void gl_shader_t::unbind() const
{
    glUseProgram(0);
}

void gl_shader_t::set_uniform_int(const std::string &name, int val)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform1i(loc, val);
}

void gl_shader_t::set_uniform_int_arr(const std::string &name, int *vals, uint32_t cnt)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform1iv(loc, cnt, vals);
}

void gl_shader_t::set_uniform_float(const std::string &name, float val)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform1f(loc, val);
}

void gl_shader_t::set_uniform_float_arr(const std::string &name, float *vals, uint32_t cnt)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform1fv(loc, cnt, vals);
}

void gl_shader_t::set_uniform_float2(const std::string &name, const glm::vec2 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform2f(loc, v.x, v.y);
}

void gl_shader_t::set_uniform_float3(const std::string &name, const glm::vec3 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform3f(loc, v.x, v.y, v.z);
}

void gl_shader_t::set_uniform_float4(const std::string &name, const glm::vec4 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void gl_shader_t::set_uniform_mat3(const std::string &name, const glm::mat3 &m)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void gl_shader_t::set_uniform_mat4(const std::string &name, const glm::mat4 &m)
{
    GLint loc = glGetUniformLocation(this->gl_id_, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void gl_shader_t::read_glsl_files(const std::string &vertex_src_path, const std::string &fragment_src_path)
{
    auto vertex_src = read_file(vertex_src_path);
    auto fragment_src = read_file(fragment_src_path);

    this->gl_id_ = glCreateProgram();

    GLuint vertex_shader = compile(GL_VERTEX_SHADER, vertex_src);
    glAttachShader(this->gl_id_, vertex_shader);
    GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_src);
    glAttachShader(this->gl_id_, fragment_shader);

    glLinkProgram(this->gl_id_);

    GLint linked = 0;
    glGetProgramiv(this->gl_id_, GL_LINK_STATUS, (int *) &linked);

    if (!linked) {
        GLint max_len = 0;
        glGetProgramiv(this->gl_id_, GL_INFO_LOG_LENGTH, &max_len);

        auto *log = new GLchar[max_len];
        glGetProgramInfoLog(this->gl_id_, max_len, &max_len, log);

        glDeleteProgram(this->gl_id_);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        throw gl_load_ex("Couldn't compile shader\n" + std::string(log));
    }

    glDetachShader(this->gl_id_, vertex_shader);
    glDetachShader(this->gl_id_, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

uint32_t gl_shader_t::compile(uint32_t type, const std::string &src)
{
    GLuint shader = glCreateShader(type);

    auto src_c_str = src.c_str();
    glShaderSource(shader, 1, &src_c_str, nullptr);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint max_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);

        auto *log = new GLchar[max_len];
        glGetShaderInfoLog(shader, max_len, &max_len, log);

        glDeleteShader(shader);

        throw gl_load_ex("Couldn't compile shader\n" + src + "\n" + std::string(log));
    }

    return shader;
}

}
