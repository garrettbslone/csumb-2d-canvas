//
// Created by Garrett on 12/8/2022.
//

#ifndef MB2DC_SAMPLES_COMMON_HPP
#define MB2DC_SAMPLES_COMMON_HPP

#include <mb2dc.hpp>
#include <iostream>

using namespace mb2dc;
using namespace glm;

// Avoid using namespace std and only use specific std types
// since std::ref and mb2dc::ref will cause ambiguous type if
// using both namespaces

// iostream
using std::cout;
using std::endl;

// containers
using std::vector;
using std::unordered_map;
using std::pair;

// string
using std::string;
using std::to_string;
using std::tolower;
using std::toupper;

// utility
using std::swap;

#endif //MB2DC_SAMPLES_COMMON_HPP
