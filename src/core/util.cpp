//
// Created by Garrett on 8/8/22.
//

#include <core/util.hpp>

#include <fstream>
#include <unordered_map>

namespace mb2dc {

std::string get_res(const std::string &res)
{
    return res_path + res;
}

int ascii_cast(char c)
{
    return static_cast<int>(static_cast<unsigned char>(c));
}


const std::unordered_map<key_code, char> shift_mapped_keys({
    {KEY_SPACE, ' '},
    {KEY_APOSTROPHE, '"'},
    {KEY_COMMA, '<'},
    {KEY_MINUS, '_'},
    {KEY_PERIOD, '>'},
    {KEY_SLASH, '?'},
    {KEY_0, ')'},
    {KEY_1, '!'},
    {KEY_2, '@'},
    {KEY_3, '#'},
    {KEY_4, '$'},
    {KEY_5, '%'},
    {KEY_6, '^'},
    {KEY_7, '&'},
    {KEY_8, '*'},
    {KEY_9, '('},
    {KEY_SEMICOLON, ':'},
    {KEY_EQUAL, '+'},
    {KEY_A, 'a'},
    {KEY_B, 'b'},
    {KEY_C, 'c'},
    {KEY_D, 'd'},
    {KEY_E, 'e'},
    {KEY_F, 'f'},
    {KEY_G, 'g'},
    {KEY_H, 'h'},
    {KEY_I, 'i'},
    {KEY_J, 'j'},
    {KEY_K, 'k'},
    {KEY_L, 'l'},
    {KEY_M, 'm'},
    {KEY_N, 'n'},
    {KEY_O, 'o'},
    {KEY_P, 'p'},
    {KEY_Q, 'q'},
    {KEY_R, 'r'},
    {KEY_S, 's'},
    {KEY_T, 't'},
    {KEY_U, 'u'},
    {KEY_V, 'v'},
    {KEY_W, 'w'},
    {KEY_X, 'x'},
    {KEY_Y, 'y'},
    {KEY_Z, 'z'},
    {KEY_LEFT_BRACKET, '{'},
    {KEY_BACKSLASH, '|'},
    {KEY_RIGHT_BRACKET, '}'},
    {KEY_GRAVE_ACCENT, '~'},
});

constexpr auto NO_PRINT_CHAR = static_cast<char>(KEY_WORLD_1);

char key_code_to_ascii(key_code key, bool is_shift, bool is_caps)
{
    auto key_u16 = static_cast<uint16_t>(key);
    if (key_u16 > static_cast<uint16_t>(std::numeric_limits<char>::max())) {
        return NO_PRINT_CHAR;
    }

    auto it = shift_mapped_keys.find(key);
    if (it == shift_mapped_keys.end()) {
        return NO_PRINT_CHAR;
    }

    if (key >= KEY_A && key <= KEY_Z) {
        if ((!is_shift && !is_caps) || (is_shift && is_caps)) {
            return it->second;
        }
    } else if (is_shift) {
        return it->second;
    }

    return static_cast<char>(key);
}

long long pow_2_greater(int n)
{
    long long p2 = 1;

    while (p2 < n && p2 < std::numeric_limits<long long>::max()) {
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