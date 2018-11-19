#pragma once

#include <cstdlib>
#include <cstdint>
#include <string>
#include <iostream>
#include <cstring>

using block_t = uint8_t*;

std::pair<uint8_t*, size_t> convert_str_to_bytes(const std::string& str) {
    auto result = new uint8_t[str.length() * 2];
    for (size_t i = 0; i < str.length(); i++) {
        result[i * 2] = ((uint8_t)str[i]) >> 4;
    }
    return std::make_pair(result, str.length() * 2);
}

void print_hex(const uint8_t* str, int length, const std::string& msg="") {
    std::cout.flags(std::ios_base::hex);
    std::cout << msg;
    for (int i = 0; i < length; i++) {
        std::cout << ((int)str[i] >> 4) << ((int)str[i] % (1<<4));
    }
    std::cout << std::endl;
}

uint8_t* concat(const uint8_t* str, int length) {
    auto result = new uint8_t[length / 2];
    for (int i = 0; i < length / 2; i++) {
        result[i] = (str[2 * i] << 4) | str[2 * i + 1];
    }
    return result;
}

uint8_t* parse_hex_string(const std::string& str) {
    auto result = new uint8_t[str.length()];
    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            result[i] = (uint8_t)(str[i] - '0');
        } else {
            result[i] = (uint8_t)(str[i] - 'a' + 10);
        }
    }
    return concat(result, (int)str.length());
}

block_t copy(const block_t block, const size_t length = 16) {
    auto result = new uint8_t[length];
    memcpy(result + length - 16, block, 16);
    return result + length - 16;
}


template <typename T>
T* add2(T *block, T *a) {
    for (int i = 0; i < 16 / sizeof(T); i++) {
        block[i] ^= a[i];
    }
    return block;
}