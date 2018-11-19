#pragma once

#include <cstdint>
#include <cstring>
#include "s_conversion.h"

uint8_t r_array[16] = {
        148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148, 1
};

uint8_t r_calculated[16][256];

__uint128_t r_calculated_best[16][256];
uint64_t r_calculated_best_64[16][256][2];
uint64_t r_calculated_best_64_keys[10][16][256][2];


uint8_t multiply(uint8_t first, uint8_t second) {
    uint32_t tresult = 0;
    for (int i = 0; i < 8; i++) {
        if (second & (1 << i)) {
            tresult ^= first << i;
        }
    }
    const uint32_t mod = 0b111000011;
    while ((tresult >> 8) > 0) {
        for (int i = 31; i > 0; i--) {
            if (tresult & (1 << i)) {
                tresult ^= mod << (i - 8);
                break;
            }
        }
    }
    return (uint8_t) tresult;
}

void init_r_conversion_table() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 256; j++) {
            r_calculated[i][j] = multiply(r_array[i], (uint8_t) j);
        }
    }
}

uint8_t *r_conversion(const uint8_t *block) {
    auto result = new uint8_t[16];
    memcpy(result + 1, block, 15);

    for (int i = 0; i < 16; i++) {
        result[0] ^= r_calculated[i][block[i]];
    }
    return result;
}

uint8_t *l_conversion(const uint8_t *block) {
    auto *result = new uint8_t[16];
    memcpy(result, block, 16);
    for (int i = 0; i < 16; i++) {
        result = r_conversion(result);
    }
    return result;
}


uint8_t *r_inv_conversion(const uint8_t *block) {
    auto result = new uint8_t[16];
    memcpy(result, block + 1, 15);

    result[15] = 0;
    for (int i = 0; i < 15; i++) {
        result[15] ^= r_calculated[i][block[i + 1]];
    }
    result[15] ^= r_calculated[15][block[0]];
    return result;
}

uint8_t *l_inv_conversion(const uint8_t *block) {
    auto *result = new uint8_t[16];
    memcpy(result, block, 16);
    for (int i = 0; i < 16; i++) {
        result = r_inv_conversion(result);
    }
    return result;
}


namespace fast {

    inline uint8_t *r_conversion_fast(uint8_t *block) {
        auto result = block - 1;
        for (int i = 0; i < 16; i++) {
            result[0] ^= r_calculated[i][block[i]];
        }
        return result;
    }

    inline uint8_t *l_conversion_fast(uint8_t *block) {
        auto result = block;
        for (int i = 0; i < 16; i++) {
            result = r_conversion_fast(result);
        }
        return result;
    }

    void init_fast_conversion(const uint8_t *pi) {
        static uint8_t procedure_array[32];
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 256; j++) {
                memset(procedure_array, 0, 32);
                procedure_array[31 - i] = pi[(uint8_t )j];

                auto begin = procedure_array + 16;
                begin = l_conversion_fast(begin);

                r_calculated_best[i][j] = *((__uint128_t*)begin);
                r_calculated_best_64[i][j][0] = (uint64_t)(r_calculated_best[i][j]);
                r_calculated_best_64[i][j][1] = (uint64_t)(r_calculated_best[i][j]>> 64);
                if (r_calculated_best[i][j] != (r_calculated_best_64[i][j][0] +
                    ((__uint128_t)(r_calculated_best_64[i][j][1])<< 64))) {
                    throw 1;
                }
            }
        }
    }

    inline uint8_t *l_conversion_fast_128_ptr(uint8_t *block) {
        auto result = (__uint128_t*)(block - 16);
        for (int i = 0; i < 16; i++) {
            *result ^= r_calculated_best[i][block[15 - i]];
        }
        return (uint8_t*)result;
    }

    inline uint8_t *l_conversion_fast_64_ptr(uint8_t *block) {
        auto result = (uint64_t*)(block - 16);
        for (int i = 0; i < 16; i++) {
            auto value = block[15 - i];
            *result ^= r_calculated_best_64[i][value][0];
            *(result + 1) ^= r_calculated_best_64[i][value][1];
        }
        return (uint8_t*)result;
    }

    inline uint8_t *r_inv_conversion_fast(uint8_t *block) {
        auto result = block + 1;

        for (int i = 0; i < 15; i++) {
            result[15] ^= r_calculated[i][result[i]];
        }
        result[15] ^= r_calculated[15][block[0]];
        return result;
    }

    inline uint8_t *l_inv_conversion_fast(uint8_t *block) {
        auto result = block;
        for (int i = 0; i < 16; i++) {
            result = r_inv_conversion_fast(result);
        }
        return result;
    }
}