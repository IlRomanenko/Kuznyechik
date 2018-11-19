#pragma once

#include "utils.h"
#include "lr_conversion.h"
#include "s_conversion.h"
#include "keys.h"


inline block_t lsx_stage(block_t input) {
    return fast::l_conversion_fast_64_ptr(input);
}

inline block_t lsx_inv_stage(block_t input, block_t key) {
    return s_conversion(fast::l_inv_conversion_fast(add2(input, key)), inv_pi);
}

uint8_t procedure_array[16 * 10];

block_t encode(block_t block) {
    auto begin = procedure_array + (16 * 10 - 16);
    memcpy(begin, block, 16);
    memset(procedure_array, 0, 16 * 9);

    for (int i = 0; i < 9; i++) {
        begin = lsx_stage(add2(begin, keys[i]));
    }
    return add2(begin, keys[9]);
}

block_t decode(block_t block) {
    static uint8_t procedure_array[16 * 10];
    auto begin = procedure_array;
    memcpy(begin, block, 16);
    memset(procedure_array + 16, 0, 16 * 9);

    for (int i = 0; i < 9; i++) {
        begin = lsx_inv_stage(begin, keys[9 - i]);
    }
    return add2(begin, keys[0]);
}