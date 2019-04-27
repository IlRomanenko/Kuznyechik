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


    auto ex = parse_hex_string("1122334455667700ffeeddccbbaa9988");

    print_hex(ex, 16);
    int SIZE = 10 * 1000 * 1000;

    auto time_begin = std::chrono::steady_clock::now();
    for (int i = 0; i < SIZE; i++) {
        ex = encode(ex);
    }
    auto time_end = std::chrono::steady_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count();
    std::cerr << "encode: " << 16.0 * SIZE / milliseconds / 1024.0 << " MB/s" << std::endl;

    time_begin = std::chrono::steady_clock::now();
    for (int i = 0; i < SIZE; i++) {
        ex = decode(ex);
    }
    time_end = std::chrono::steady_clock::now();

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
    auto res = parse_hex_string("336f4d296059fbe3");
    print_hex(ex, 16 * 4);
    print_hex(OMAC(ex, 16 * 4, 16), 8);
    print_hex(res, 8);
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

    auto res = parse_hex_string("F195D8BEC10ED1DBD57B5FA240BDA1B88"
                                "5EEE733F6A13E5DF33CE4B33C45DEE44B"
                                "CEEB8F646F4C55001706275E85E800587"
                                "C4DF568D094393E4834AFD0805046CF30"
                                "F57686AEECE11CFC6C316B8A896EDFFD0"
                                "7EC813636460C4F3B743423163E6409A9"
                                "C282FAC8D469D221E7FBD6DE5D");
    print_hex(ex, 16 * 7);
    print_hex(ctr(ex, 16 * 7, 32, IV), 16 * 7);
    print_hex(res, 16 * 7);

}

int main() {

//    test_all();
//    test_speed();
    test_omac();
//    test_ctr();
    return 0;
}