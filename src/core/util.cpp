//
// Created by Garrett on 8/8/22.
//

#include <core/util.hpp>

#include <fstream>

namespace mb2dc {

std::string get_res(const std::string &res)
{
    return res_path + res;
}

int ascii_cast(char c)
{
    return static_cast<int>(static_cast<unsigned char>(c));
}

long long pow_2_greater(int n)
{
    long long p2 = 1;

    while (p2 < n && p2 < LONG_LONG_MAX) {
        p2 = p2 << 1;
    }

    return p2;
}

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