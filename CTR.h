#pragma once

#include <vector>
#include <cassert>

#include "utils.h"
#include "kuznyechick.h"



__uint128_t encode128(const __uint128_t value) {
    static uint8_t memory[16];
    deconvert128(value, memory);
    memcpy(memory, encode(memory), 16);
    return cast_reversed(memory);
}


void acpkm() {
    auto master_key = new uint8_t[16];

    __uint128_t D0 = cast_reversed(parse_hex_string("808182838485868788898A8B8C8D8E8F"));
    __uint128_t D1 = cast_reversed(parse_hex_string("909192939495969798999A9B9C9D9E9F"));

    deconvert128(encode128(D0), master_key);
    deconvert128(encode128(D1), master_key + 16);

    print_hex(master_key, 32);
    init_keys(master_key);
}

uint8_t* ctr(uint8_t* msg, size_t msg_len, size_t blockSize, uint8_t* IV) {

    auto IV_cp = new uint8_t[16];
    memset(IV_cp, 0, 16);
    memcpy(IV_cp, IV, 8);

    std::vector<__uint128_t> v_msg;
    for (size_t i = 0; i < msg_len / 16; i++) {
        v_msg.push_back(cast_reversed(msg + 16 * i));
    }

    std::vector<__uint128_t> enc_msg;


    blockSize /= 16;
    size_t total_blocks = v_msg.size() / blockSize;
    if (v_msg.size() % blockSize != 0) {
        total_blocks += 1;
    }

    print_hex(IV_cp, 16, "before cycle ");
    __uint128_t ctr = cast_reversed(IV_cp);
    auto print_arr = new uint8_t[16];

    print_hex(IV_cp, 16, "0_ ");


    memset(print_arr, 0, 16);
    deconvert128(ctr, print_arr);
    print_hex(print_arr, 16, "1_ ");

    auto encoded_ctr = encode128(ctr);

    memset(print_arr, 0, 16);
    deconvert128(encoded_ctr, print_arr);
    print_hex(print_arr, 16, "2_ ");


    memset(print_arr, 0, 16);
    memcpy(print_arr, encode(IV_cp), 16);
    print_hex(print_arr, 16, "3_ ");





    for (size_t block_id = 0; block_id < total_blocks; block_id++) {
        for (size_t block_msg_id = 0; block_msg_id < blockSize; block_msg_id++) {
            if (block_id * blockSize + block_msg_id == v_msg.size()) {
                break;
            }
            auto encoded_ctr = encode128(ctr);


            memset(print_arr, 0, 16);
            deconvert128(encoded_ctr, print_arr);
            print_hex(print_arr, 16);

            memset(print_arr, 0, 16);
            deconvert128(v_msg[block_id * blockSize + block_msg_id], print_arr);
            print_hex(print_arr, 16);

            enc_msg.push_back(v_msg[block_id * blockSize + block_msg_id] ^ encoded_ctr);

            memset(print_arr, 0, 16);
            deconvert128(enc_msg.back(), print_arr);
            print_hex(print_arr, 16);

            ctr += 1;
        }
        acpkm();
    }

    delete[] IV_cp;
    auto encoded_msg = new uint8_t[msg_len];

    for (size_t i = 0; i < enc_msg.size(); i++) {
        deconvert128(enc_msg[i], encoded_msg + i * 16);
    }
    return encoded_msg;
}