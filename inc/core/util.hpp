//
// Created by Garrett on 8/5/22.
//

#ifndef MB2DC_UTIL_HPP
#define MB2DC_UTIL_HPP

#include "exception.hpp"
#include "key_codes.hpp"

#include <glm/ext/scalar_constants.hpp>

#include <memory>

namespace mb2dc {

const std::string res_path(RES_PATH);

std::string get_res(const std::string &res);

template<typename T>
using ref_t = std::shared_ptr<T>;

template<class T, class ...Args>
ref_t<T> new_ref(Args &&... args)
{
    return std::make_shared<T>(args ...);
}

/*
 * Cast a char to its int value.
 */
int ascii_cast(char c);

[[nodiscard]] char key_code_to_ascii(key_code key, bool is_shift, bool is_caps);

long long pow_2_greater(int n);

/*
 * Read the contents of 'f_path' as a string.
 */
std::string read_file(const std::string &f_path);

constexpr auto pi = glm::pi<double>();
constexpr auto fpi = glm::pi<float>();

#define UNUSED(x) static_cast<void>(x);

}

#endif //MB2DC_UTIL_HPP
