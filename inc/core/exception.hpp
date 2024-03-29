//
// Created by Garrett on 9/19/21.
//

#ifndef MB2DC_EXCEPTION_HPP
#define MB2DC_EXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>

namespace mb2dc {

class mb2dc_runtime_ex : public std::exception {
public:
    std::string msg_{};
    explicit mb2dc_runtime_ex(std::string msg)
        : msg_(std::move(msg)) {}
    ~mb2dc_runtime_ex() override = default;
};

class out_of_bounds_ex : public mb2dc_runtime_ex {
public:
    explicit out_of_bounds_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class io_ex : public mb2dc_runtime_ex {
public:
    explicit io_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class texture_ex : public mb2dc_runtime_ex {
public:
    explicit texture_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class viewport_ex : public mb2dc_runtime_ex {
public:
    explicit viewport_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class gl_load_ex : public mb2dc_runtime_ex {
public:
    explicit gl_load_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class canvas_ex : public mb2dc_runtime_ex {
public:
    explicit canvas_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class drawable_ex : public mb2dc_runtime_ex {
public:
    explicit drawable_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class font_load_ex : public mb2dc_runtime_ex {
public:
    explicit font_load_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class font_manager_ex : public mb2dc_runtime_ex {
public:
    explicit font_manager_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

class text_ex : public mb2dc_runtime_ex {
public:
    explicit text_ex(const std::string &msg)
            : mb2dc_runtime_ex(msg) {}
};

}

#endif //MB2dc_EXCEPTION_HPP
