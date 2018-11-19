#pragma once

#include "utils.h"
#include "s_conversion.h"
#include "lr_conversion.h"

block_t c_array[33];
block_t keys[10];
__uint128_t keys_128[10];

void init_c_array() {
    for (int i = 1; i <= 32; i++) {
        c_array[i] = new uint8_t[16];
        c_array[i][15] = (uint8_t)i;
        c_array[i] = l_conversion(c_array[i]);
    }
}

std::pair<block_t, block_t> f_conversion(block_t key, block_t a1, block_t a0) {
    return std::make_pair(add2(l_conversion(s_conversion(add2(key, a1), pi)), a0), a1);
}

void init_keys(const uint8_t *master_key) {
    for (auto &key : keys) {
        key = new uint8_t[16];
    }

    memcpy(keys[0], master_key, 16);
    memcpy(keys[1], master_key + 16, 16);

    for (int i = 1; i <= 4; i++) {
        keys[2 * i] = keys[2 * i - 2];
        keys[2 * i + 1] = keys[2 * i - 1];
        for (int j = 1; j <= 8; j++) {
            std::pair<uint8_t *, uint8_t *> pkeys = f_conversion(copy(c_array[8 * (i - 1) + j]), keys[2 * i], keys[2 * i + 1]);
            keys[2 * i] = pkeys.first;
            keys[2 * i + 1] = pkeys.second;
        }
    }

    for (int i = 0; i < 10; i++) {
        keys_128[i] = *((__uint128_t*)keys[i]);
    }
}
