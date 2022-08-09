//
// Created by Garrett on 8/5/22.
//

#ifndef MB2DC_UTIL_HPP
#define MB2DC_UTIL_HPP

#include "exception.hpp"

#include <memory>

namespace mb2dc {

template<typename T>
using ref = std::shared_ptr<T>;

template<class T, class ...Args>
ref<T> new_ref(Args &&... args)
{
    return std::make_shared<T>(args ...);
}

/*
* Read the contents of 'f_path' as a string.
*/
std::string read_file(const std::string &f_path);

}

#endif //MB2DC_UTIL_HPP
