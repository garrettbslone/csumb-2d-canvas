//
// Created by Garrett on 8/8/22.
//

#include <core/util.hpp>

#include <fstream>

namespace mb2dc {

std::string read_file(const std::string &f_path)
{
    std::ifstream fin(f_path, std::ifstream::in | std::ifstream::binary);

    if (!fin) {
        throw io_ex("Couldn't read file: " + f_path);
    }

    fin.seekg(0, std::ifstream::end);
    std::string res(static_cast<size_t>(fin.tellg()), '\0');

    fin.seekg(0, std::ifstream::beg);
    fin.read(&res[0], res.size());
    fin.close();

    return res;
}

}