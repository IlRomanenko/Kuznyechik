#pragma once

#include "keys.h"
#include "lr_conversion.h"
#include "s_conversion.h"
#include "utils.h"


/**
 * 00112233445566778899aabbccddeeff -> fc7765aeea0c9a7ed7e8387d88d86cb6
 */
void test_s_conversion() {

    std::cout << "s_conversion" << std::endl << std::endl;
    uint8_t *str = parse_hex_string("00112233445566778899aabbccddeeff");
    int length = 16;

    print_hex(str, length);
    print_hex(s_conversion(copy(str), pi), length);
    print_hex(s_conversion(s_conversion(copy(str), pi), inv_pi), length);
}

/**
 * 00000000000000000000000000000100
 * 94000000000000000000000000000001
 * a5940000000000000000000000000000
 * 64a59400000000000000000000000000
 */

/**
 * 94000000000000000000000000000001
 * a5940000000000000000000000000000
 * 64а59400000000000000000000000000
 * 0d64a594000000000000000000000000
 */
void test_r_conversion() {
    int length = 16;
    std::cout << "r_conversion" << std::endl << std::endl;
    uint8_t *ex1 = parse_hex_string("00000000000000000000000000000100");
    uint8_t *ex2 = parse_hex_string("94000000000000000000000000000001");
    uint8_t *ex3 = parse_hex_string("a5940000000000000000000000000000");
    uint8_t *ex4 = parse_hex_string("64a59400000000000000000000000000");

    print_hex(ex1, length);
    print_hex(ex2, length);
    print_hex(ex3, length);
    print_hex(ex4, length);

    std::cout << std::endl;

    print_hex(r_conversion(ex1), length);
    print_hex(r_conversion(ex2), length);
    print_hex(r_conversion(ex3), length);
    print_hex(r_conversion(ex4), length);

    std::cout << std::endl;
    print_hex(r_inv_conversion(r_conversion(ex1)), length);
    print_hex(r_inv_conversion(r_conversion(ex2)), length);
    print_hex(r_inv_conversion(r_conversion(ex3)), length);
    print_hex(r_inv_conversion(r_conversion(ex4)), length);
}

/**
 * L(64a59400000000000000000000000000) = d456584dd0e3e84cc3166e4b7fa2890d,
 * L(d456584dd0e3e84cc3166e4b7fa2890d) = 79d26221b87b584cd42fbc4ffea5de9a,
 * L(79d26221b87b584cd42fbc4ffea5de9a) = 0e93691a0cfc60408b7b68f66b513c13,
 * L(0e93691a0cfc60408b7b68f66b513c13) = e6a8094fee0aa204fd97bcb0b44b8580.
 */
void test_l_conversion() {

    std::cout << "l_conversion" << std::endl << std::endl;
    int length = 16;
    uint8_t *ex1 = parse_hex_string("64a59400000000000000000000000000");
    uint8_t *ex2 = parse_hex_string("d456584dd0e3e84cc3166e4b7fa2890d");
    uint8_t *ex3 = parse_hex_string("79d26221b87b584cd42fbc4ffea5de9a");
    uint8_t *ex4 = parse_hex_string("0e93691a0cfc60408b7b68f66b513c13");

    print_hex(ex1, length);
    print_hex(ex2, length);
    print_hex(ex3, length);
    print_hex(ex4, length);

    std::cout << std::endl;

    print_hex(l_conversion(copy(ex1)), length, "d456584dd0e3e84cc3166e4b7fa2890d  ");
//    print_hex(fast::l_conversion_fast_64_ptr(copy(ex1, 32)), length, "d456584dd0e3e84cc3166e4b7fa2890d  ");
    print_hex(fast::l_conversion_fast_128_ptr(copy(ex2, 32)), length, "79d26221b87b584cd42fbc4ffea5de9a  ");
    print_hex(l_conversion(ex3), length);
    print_hex(l_conversion(ex4), length);

    std::cout << std::endl;
    print_hex(l_inv_conversion(l_conversion(ex1)), length);
    print_hex(l_inv_conversion(l_conversion(ex2)), length);
    print_hex(l_inv_conversion(l_conversion(ex3)), length);
    print_hex(l_inv_conversion(l_conversion(ex4)), length);
}

void test_all() {

    init_r_conversion_table();
    init_c_array();
    init_keys(parse_hex_string("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef"));
    fast::init_fast_conversion(pi);

    std::cout << std::endl << std::endl;
    for (auto key : keys) {
        print_hex(key, 16);
    }

    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;

    test_s_conversion();

    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;

    test_r_conversion();

    std::cout << std::endl << std::endl;
    std::cout << std::endl << std::endl;

    test_l_conversion();

}
