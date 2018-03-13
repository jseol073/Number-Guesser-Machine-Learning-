//
// Created by John Seol on 3/10/18.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const string test_image =
        "                            \n"
        "                            \n"
        "                            \n"
        "                            \n"
        "                            \n"
        "                            \n"
        "                            \n"
        "             ++###+         \n"
        "             ######+        \n"
        "            +######+        \n"
        "            ##+++##+        \n"
        "           +#+  +##+        \n"
        "           +##++###+        \n"
        "           +#######+        \n"
        "           +#######+        \n"
        "            +##+###         \n"
        "              ++##+         \n"
        "              +##+          \n"
        "              ###+          \n"
        "            +###+           \n"
        "            +##+            \n"
        "           +##+             \n"
        "          +##+              \n"
        "         +##+               \n"
        "         ##+                \n"
        "        +#+                 \n"
        "        +#+                 \n"
        "                            \n"
        "                            ";



TEST_CASE("2D image") {
    vector<string> image_test;

}

TEST_CASE("Class Frequency") {
    vector<int> label_test = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
    map<int, long> frequency_expected;
    //map<int, long> frequency_actual = main::countFrequency(label_test);
}
