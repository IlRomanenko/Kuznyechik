#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <cstdlib>
#include <chrono>

#include "kuznyechick.h"
#include "tests.h"
#include "OMAC.h"
#include "CTR.h"
#include <ctime>


void test_speed() {

    init_r_conversion_table();
    fast::init_fast_conversion(pi);
    init_c_array();
    init_keys(parse_hex_string("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef"));
//    fast::init_with_keys(keys);


    auto ex = parse_hex_string("1122334455667700ffeeddccbbaa9988");

    print_hex(ex, 16);
//    print_hex(decode(encode(ex)), 16);
//    print_hex(decode(decode(encode(encode(ex)))), 16);
//    std::cout << std::endl<< std::endl;;

    int SIZE = 10 * 1000 * 1000;

    auto time_begin = std::chrono::steady_clock::now();
//    auto time_begin = clock();
    for (int i = 0; i < SIZE; i++) {
        ex = encode(ex);
    }
//    auto time_end = clock();
    auto time_end = std::chrono::steady_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count();
    std::cerr << "encode: " << 16.0 * SIZE / milliseconds / 1024.0 << " MB/s" << std::endl;
//    auto seconds = (time_end - time_begin) / (double)CLOCKS_PER_SEC;
//    std::cerr << "encode: " << 16.0 * SIZE / seconds / 1024.0 / 1024.0 << " MB/s" << std::endl;

    time_begin = std::chrono::steady_clock::now();
//    time_begin = clock();
    for (int i = 0; i < SIZE; i++) {
        ex = decode(ex);
    }
//    time_end = clock();
    time_end = std::chrono::steady_clock::now();
//    seconds = (time_end - time_begin) / CLOCKS_PER_SEC;
//    std::cerr << "decode: " << 16.0 * SIZE / seconds / 1024.0 / 1024.0 << " MB/s" << std::endl;
//
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count();
    std::cerr << "decode: " << 16.0 * SIZE / milliseconds / 1024.0 << " MB/s" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    print_hex(ex, 16, "decoded ");

}

void test_omac() {
    init_r_conversion_table();
    fast::init_fast_conversion(pi);
    init_c_array();
    init_keys(parse_hex_string("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef"));
    init_OMAC();

    auto ex = parse_hex_string("1122334455667700ffeeddccbbaa998800112233445566778899aabbcceeff0a112233445566778899aabbcceeff0a002233445566778899aabbcceeff0a0011");
    print_hex(ex, 16 * 4);
    print_hex(OMAC(ex, 16 * 4, 16), 16);
}

void test_ctr() {

    init_r_conversion_table();
    fast::init_fast_conversion(pi);
    init_c_array();
    init_keys(parse_hex_string("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef"));

    auto IV = parse_hex_string("1234567890ABCEF0");
    auto ex = parse_hex_string("1122334455667700FFEEDDCCBBAA9988"\
                               "00112233445566778899AABBCCEEFF0A"\
                               "112233445566778899AABBCCEEFF0A00"\
                               "2233445566778899AABBCCEEFF0A0011"\
                               "33445566778899AABBCCEEFF0A001122"\
                               "445566778899AABBCCEEFF0A00112233"\
                               "5566778899AABBCCEEFF0A0011223344");
    print_hex(ex, 16 * 7);
    print_hex(ctr(ex, 16 * 7, 32, IV), 16 * 7);

}

int main() {

//    test_all();
//    test_speed();
    test_omac();
//    test_ctr();
    return 0;
}