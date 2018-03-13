//
// Created by John Seol on 3/13/18.
//

#include "Training.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>
#include <fstream>

using namespace std;

const int IMAGE_LENGTH = 28;

const int CLASS_SIZE = 10;

const double K_VALUE = 8;

const int V = 2;

/**
 * Takes each line of file and creates a 2D string vector in which each the first index corresponds with the image
 * @param all_line, a vector that contain all the lines of the trainingimages file
 * @return 2d vector<string> that has all the images as the first index and the lines as the second index
 */
vector<vector<string>> Training::eachImageVector(vector<string> all_lines) {
    vector<vector<string>> all_images;
    all_images.resize((all_lines.size() / IMAGE_LENGTH));

    int imageIndex = -1;
    for (int line_index = 0; line_index < all_lines.size(); line_index++) {
        if (line_index % IMAGE_LENGTH > 0) {
            all_images[imageIndex].push_back(all_lines[line_index]);
        } else {
            imageIndex++;
        }
    }
    return all_images;
}

/**
 * Takes each line of file and makes a 2D boolean vector
 * @param each_line
 * @return
 */
vector<vector<bool>> Training::strToBinaryImage(vector<string> each_image) {
    vector<vector<bool>> binary_2d;
    binary_2d.resize(each_image.size());

    for (int line_index = 0; line_index < each_image.size(); line_index++) {

        if (!each_image[line_index].empty()) {
            binary_2d[line_index].resize(IMAGE_LENGTH);

            for (int pos = 0; pos < IMAGE_LENGTH; pos++) {
                if (each_image[line_index].at(pos) == '+') {
                    binary_2d[line_index][pos] = true;

                } else if (each_image[line_index].at(pos) == '#') {
                    binary_2d[line_index][pos] = true;

                } else if (each_image[line_index].at(pos) == ' ') {
                    binary_2d[line_index][pos] = false;
                }
            }
        } else {
            continue;
        }
    }
    return binary_2d;
}

/**
 *
 * @param label_vector
 * @return
 */
vector<int> Training::getLabel(vector<string> label_vector) {
    vector<int> int_labels;
    for (const auto &label : label_vector) {
        stringstream num(label);
        int image_value = 0;
        num >> image_value;
        int_labels.push_back(image_value);
    }
    return int_labels;
}

/**
 *
 * @param labels
 * @return
 */
map<int, long> Training::countFrequency(vector<int> label_vector) {
    map<int, long> frequency_class;
    for (int i = 0; i < CLASS_SIZE; i++) {
        frequency_class.insert(pair <int, long> (i, count(label_vector.begin(), label_vector.end(), i)));
    }
    return frequency_class;
}

/**
 *
 * @param binary_images
 * @param label_vector
 * @return
 */
vector<vector<int>> Training::getForegroundCount(vector<BinaryImage> binary_images, vector<int> label_vector) {
    vector<vector<int>> count_vector;
    count_vector.resize(CLASS_SIZE);
    int count = 0;

    for (int class_ = 0; class_ < CLASS_SIZE; class_++) {

        for (int lineIndex = 0; lineIndex < (IMAGE_LENGTH - 1); lineIndex++) {

            for (int boolIndex = 0; boolIndex < IMAGE_LENGTH; boolIndex++) {
                count = 0;

                for (int objIndex = 0; objIndex < binary_images.size(); objIndex++) {

                    if (label_vector[objIndex] == class_) {

                        if (binary_images[objIndex].getBinaryImage()[lineIndex][boolIndex]) {
                            count++;
                        }
                    }
                }
//                cout << "count: " << count << " class: " << class_ << ", lineIndex: " << lineIndex << ", boolIndex: "
//                     << boolIndex << '\n';
                count_vector[class_].push_back(count);

            }
        }
    }
    return count_vector;
}

/**
 *
 * @param binary_images
 * @param label_vector
 * @return
 */
vector<vector<int>> Training::getBackgroundCount(vector<BinaryImage> binary_images, vector<int> label_vector) {
    vector<vector<int>> count_vector;
    count_vector.resize(CLASS_SIZE);
    int count = 0;

    for (int class_ = 0; class_ < CLASS_SIZE; class_++) {

        for (int lineIndex = 0; lineIndex < (IMAGE_LENGTH - 1); lineIndex++) {

            for (int boolIndex = 0; boolIndex < IMAGE_LENGTH; boolIndex++) {
                count = 0;

                for (int objIndex = 0; objIndex < binary_images.size(); objIndex++) {

                    if (label_vector[objIndex] == class_) {

                        if (!binary_images[objIndex].getBinaryImage()[lineIndex][boolIndex]) {
                            count++;
                        }
                    }
                }
//                cout << "count: " << count << " class: " << class_ << ", lineIndex: " << lineIndex << ", boolIndex: "
//                     << boolIndex << '\n';
                count_vector[class_].push_back(count);

            }
        }
    }
    return count_vector;
}

/**
 *
 * @param count1_each_class
 * @param frequency_class
 * @return
 */
vector<vector<double>> Training::getConditionalProbFor1(vector<vector<int>> count1_each_class, map<int, long> frequency_class) {
    vector<vector<double>> conditional_prob_1;
    conditional_prob_1.resize(CLASS_SIZE);

    for (int curr_class = 0; curr_class < count1_each_class.size(); curr_class++) {
        for (int curr_coordinate = 0; curr_coordinate < count1_each_class[curr_class].size(); curr_coordinate++) {
            double cond_prob = (K_VALUE + count1_each_class[curr_class][curr_coordinate])
                               / ((V * K_VALUE) + frequency_class.at(curr_class));
            conditional_prob_1[curr_class].push_back(cond_prob);
            //cout << "Probability: " << cond_prob << ", class: " << curr_class << ", coordinate: " << curr_coordinate << '\n';

        }
    }
    return conditional_prob_1;
}

/**
 *
 * @param count0_each_class
 * @param frequency_class
 * @return
 */
vector<vector<double>> Training::getConditionalProbFor0(vector<vector<int>> count0_each_class, map<int, long> frequency_class) {
    vector<vector<double>> conditional_prob_0;
    conditional_prob_0.resize(CLASS_SIZE);

    for (int curr_class = 0; curr_class < count0_each_class.size(); curr_class++) {
        for (int curr_coordinate = 0; curr_coordinate < count0_each_class[curr_class].size(); curr_coordinate++) {

            double cond_prob = (K_VALUE + count0_each_class[curr_class][curr_coordinate])
                               / ((V * K_VALUE) + frequency_class.at(curr_class));
            conditional_prob_0[curr_class].push_back(cond_prob);
        }
    }
    return conditional_prob_0;
}

void Training::makeConditionalProbFile(vector<vector<double>> cond_prob_1) {
    std::ofstream outfile("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities");

    for (auto class_vector: cond_prob_1) {
        for (auto cond_prob : class_vector) {
            outfile << cond_prob << endl;
        }
    }
    outfile.close();
}