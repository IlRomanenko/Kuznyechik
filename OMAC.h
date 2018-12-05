#pragma once

#include "kuznyechick.h"

static block_t OMAC_key1, OMAC_key2;

void init_OMAC() {
    __uint128_t B128 = 0b10000111;

    auto R = new uint8_t[16];
    memset(R, 0, 16);
    OMAC_key1 = new uint8_t[16];
    OMAC_key2 = new uint8_t[16];

    R = encode(R);
    print_hex(R, 16);
    __uint128_t Ruint = cast_reversed(R);


    __uint128_t K1 = (Ruint << 1);
    K1 ^= ((__int128_t)Ruint < 0 ? B128 : 0);

    __uint128_t K2 = K1 << 1;
    K2 ^= ((__int128_t)K1 < 0 ? B128 : 0);

    deconvert128(K1, R);
    memcpy(OMAC_key1, R, 16);

    deconvert128(K2, R);
    memcpy(OMAC_key2, R, 16);

    print_hex(OMAC_key1, 16);
    print_hex(OMAC_key2, 16);

}

block_t OMAC(block_t data, int length, int s) {
    static uint8_t C[16];
    memset(C, 0, 16);

    int n = length / 16;
    auto copied_data = new uint8_t[(n + (length % 16 ? 1 : 0)) * 16];
    memcpy(copied_data, data, length);
    if (length % 16 == 0) {
        n--;
    } else {
        copied_data[length + 1] = 0b1000000;
    }

    for(int i = 0; i < n; i++) {
        auto encoded = encode(add2(C, copied_data + i * 16));
        memcpy(C, encoded, 16);
    }
    uint8_t *premac = nullptr;
    if (length % 16 == 0) {
        premac = encode(add2(add2(C, copied_data + n * 16), OMAC_key1));
    } else {
        premac = encode(add2(add2(C, copied_data + n * 16), OMAC_key2));
    }

    delete[] copied_data;

    return premac;
}