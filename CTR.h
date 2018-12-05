#pragma once

#include <vector>
#include <cassert>

#include "utils.h"
#include "kuznyechick.h"

__uint128_t convert128(const uint8_t* data) {
    return *((__uint128_t*)data);
}

void deconvert128(const __uint128_t value, uint8_t *data) {
    memcpy(data, (uint8_t*)(&value), 16);
}

__uint128_t encode128(const __uint128_t value) {
    static uint8_t memory[16];
    deconvert128(value, memory);
    memcpy(memory, encode(memory), 16);
    return convert128(memory);
}


uint8_t* acpkm(uint8_t* data) {

    return nullptr;
}

uint8_t* ctr(uint8_t* msg, size_t msg_len, size_t blockSize, uint8_t* IV) {

    auto IV_cp = new uint8_t[16];
    memset(IV_cp, 0, 16);
    memcpy(IV_cp, IV, 8);

    std::vector<__uint128_t> v_msg;
    for (size_t i = 0; i < msg_len; i += 16) {
        v_msg.push_back(convert128(msg + 16 * i));
    }

    std::vector<__uint128_t> enc_msg;


    size_t total_blocks = v_msg.size() / blockSize;

    __uint128_t ctr = convert128(IV_cp) << 64;
    for (size_t block_id = 0; block_id < total_blocks; block_id++) {
        for (size_t block_msg_id = 0; block_msg_id < blockSize; block_msg_id++) {
//            enc_msg.push_back(v_msg[block_id * blockSize + block_msg_id] ^ encode128(ctr));
            ctr += 1;
        }

    }



    return nullptr;
}