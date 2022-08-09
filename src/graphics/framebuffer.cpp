//
// Created by Garrett on 10/27/2021.
//
#include <graphics/framebuffer.hpp>

#include <glad/gl.h>

namespace mb2dc {

void _framebuffer_resize_cb(int width, int height)
{
    glViewport(0, 0, width, height);
}

gl_framebuffer::gl_framebuffer()
{
    glGenFramebuffers(1, &this->gl_id_);
    this->resize_cb_ = &_framebuffer_resize_cb;
    this->clear();
}

gl_framebuffer::~gl_framebuffer()
{
    glDeleteFramebuffers(1, &this->gl_id_);
}

void gl_framebuffer::init()
{
    this->reset();
    this->resize_cb_ = &_framebuffer_resize_cb;
}

void gl_framebuffer::clear()
{
    glClearColor(this->clear_clr_.r, this->clear_clr_.g, this->clear_clr_.b, this->clear_clr_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gl_framebuffer::destroy()
{
    if (this->gl_id_) {
        glDeleteFramebuffers(1, &this->gl_id_);
        this->gl_id_ = 0;
    }
}

void gl_framebuffer::reset()
{
    this->destroy();
    this->clear();

    glGenFramebuffers(1, &this->gl_id_);
}

void gl_framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->gl_id_);
    this->resize_cb_(this->width_, this->height_);
}

void gl_framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_framebuffer::resize(uint32_t width, uint32_t height)
{
    this->resize_cb_(width, height);
}

}