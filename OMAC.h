#pragma once

#include "kuznyechick.h"

static block_t OMAC_key1, OMAC_key2;

void init_OMAC() {
    uint8_t B[16];
    memset(B, 0, 16);
    *(B + 15) = 0b1010111;

    auto R = new uint8_t[16];
    OMAC_key1 = new uint8_t[16];
    OMAC_key2 = new uint8_t[16];

    R = encode(R);
    print_hex(R, 16);
    __uint128_t Ruint = *((__uint128_t*)R);

    auto t1 = (uint64_t )(Ruint >> 64);
    auto t2 = (uint64_t )((Ruint << 64) >> 64);

    auto t3 = (__uint128_t)t1 << 64 | t2;
    auto t4 = t3 * 2;
    auto t5 = (__uint128_t)(t1 * 2) << 64 | (t2 * 2);

    R = (uint8_t*)(&Ruint);
    print_hex(R, 16);
    if ((Ruint >> 127) == 0) {
        *((__uint128_t*)OMAC_key1) = Ruint << 1;
    } else {
        auto tt = Ruint * 2;
        R = (uint8_t*)(&tt);
        print_hex(R, 16);
        memcpy(OMAC_key1, add2(R, B), 16);
    }

    print_hex(OMAC_key1, 16);
    memcpy(R, OMAC_key1, 16);

    Ruint = *((__uint128_t*)R);
    if ((Ruint >> 127) == 0) {
        *((__uint128_t*)OMAC_key2) = Ruint << 1;
    } else {
        Ruint = Ruint << 1;
        R = (uint8_t*)(&Ruint);
        memcpy(OMAC_key2, add2(R, B), 16);
    }
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
        premac = encode(add2(add2(C, copied_data + (n + 1) * 16), OMAC_key1));
    } else {
        premac = encode(add2(add2(C, copied_data + (n + 1) * 16), OMAC_key2));
    }

    delete[] copied_data;

    return premac;
}