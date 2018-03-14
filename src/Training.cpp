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
 * Takes each line of each_image and makes a 2D boolean vector in which all whitespaces are assigned false
 * and anything else will be true
 * @param each_image, a vector that contains the lines of a specific image
 * @return a 2d bool vector that represents each_image but assigns true to # or + and false to any whitespace
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
 * Takes label_vector and converts each element into an int
 * @param label_vector, each element is a line from the traininglabels file
 * @return returns vector<int> which is label_vector but as int
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
 * counts the frequency of each class digit by iterating through class digits and then
 * inserting the frequencies of label_values as values of map<int, long>
 * @param labels_values, the return vector of method getLabel
 * @return, a map<int, long>, keys are class digits, values are the frequencies
 */
map<int, long> Training::countFrequency(vector<int> label_values) {
    map<int, long> frequency_class;
    for (int i = 0; i < CLASS_SIZE; i++) {
        frequency_class.insert(pair <int, long> (i, count(label_values.begin(), label_values.end(), i)));
    }
    return frequency_class;
}

/**
 * Same as the below method but counting the non-whitespaces
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
                count_vector[class_].push_back(count);
            }
        }
    }
    return count_vector;
}

/**
 * Counts all the whitespaces of a specific pixel of all the binary_images and stores them in a 2d vector of int
 * @param binary_images, all the training images in binary
 * @param label_vector, represents the int values of each line from traininglabels file
 * @return a 2d vector of int in which the first brackets represents each class digit and the second brackets
 * represents the total count of each pixel that are whitespaces
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
                count_vector[class_].push_back(count);
            }
        }
    }
    return count_vector;
}

/**
 * same as the below method but calculating the conditional probabilities for the non-whitespaces
 * @param count1_each_class
 * @param frequency_class, a map that contains the frequency of each class digit
 * @return
 */
vector<vector<double>> Training::getConditionalProbFor1(vector<vector<int>> count1_each_class,
                                                        map<int, long> frequency_class) {
    vector<vector<double>> conditional_prob_1;
    conditional_prob_1.resize(CLASS_SIZE);

    for (int curr_class = 0; curr_class < count1_each_class.size(); curr_class++) {
        for (int curr_coordinate = 0; curr_coordinate < count1_each_class[curr_class].size(); curr_coordinate++) {

            double cond_prob = (K_VALUE + count1_each_class[curr_class][curr_coordinate])
                               / ((V * K_VALUE) + frequency_class.at(curr_class));
            conditional_prob_1[curr_class].push_back(cond_prob);
        }
    }
    return conditional_prob_1;
}

/**
 * calculates the conditional probability of for each pixel of each binary image by iterating through the class digits
 * and the coordinates of each pixel
 * @param count0_each_class, the return value of getBackGroundCount method
 * @param frequency_class, a map that contains the frequency of each class digit
 * @return a 2d vector of double in which the first brackets refer to the class digit and the second brackets refer to
 * the conditional probability of each pixel
 */
vector<vector<double>> Training::getConditionalProbFor0(vector<vector<int>> count0_each_class,
                                                        map<int, long> frequency_class) {
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

/**
 * Creates and stores the total counts of non-whitespaces in a new file
 * @param cond_prob_1, a 2d vector of double which is the output of the above method
 */
void Training::makeConditionalProbFileFor1(vector<vector<double>> cond_prob_1) {
    ofstream outfile("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities1");

    for (auto class_vector: cond_prob_1) {
        for (auto cond_prob : class_vector) {
            outfile << cond_prob << endl;
        }
    }
    outfile.close();
}

/**
 * Same as above but creates another file that has the total counts of whitespaces
 * @param cond_prob_0
 */
void Training::makeConditionalProbFileFor0(vector<vector<double>> cond_prob_0) {
    ofstream outfile("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities0");

    for (auto class_vector: cond_prob_0) {
        for (auto cond_prob : class_vector) {
            outfile << cond_prob << endl;
        }
    }
    outfile.close();
}