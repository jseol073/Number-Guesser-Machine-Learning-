//
// Created by John Seol on 3/10/18.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../src/Training.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

Training training;

TEST_CASE("2D image") {
    vector<string> image_test;
    ifstream testFileStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ImageForTestCases");
    string line;
    while (getline(testFileStream, line)) {
        image_test.push_back(line);
    }
    vector<vector<string>> expected = {{}};

}

TEST_CASE("String to binary vector") {
    vector<string> input = {"  ### +# ", " #   +"};
    vector<vector<bool>> expected = {{false, false, true, true, true, false, true, true, false},
                                     {false, true, false, false, false, true}};
    REQUIRE(expected == training.strToBinaryImage(input));
}

TEST_CASE("getLabels test") {
    vector<string> labels = {"1", "2", "3"};
    vector<int> expected = {1, 2, 3};
    REQUIRE(expected == training.getLabel(labels));
}

TEST_CASE("Class Frequency") {
    vector<int> label_test = {1, 1, 2, 3, 4, 5};
    map<int, long> frequency_expected;// = {{1, 2}, {1, 2} {2, 1}, {3, 1}, {4, 1}, {5, 1}};
    map<int, long> frequency_actual = training.countFrequency(label_test);
    REQUIRE(frequency_expected == frequency_actual);
}


