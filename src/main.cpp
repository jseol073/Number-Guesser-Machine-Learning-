//
// Created by John Seol on 3/10/18.
//

//#include "main.h"
#include "BinaryImage.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const int IMAGE_LENGTH = 28;

const int CLASS_SIZE = 10;

const double K_VALUE = 8;

const int V = 2;

vector<vector<string>> eachImageVector(vector<string> by_line);

vector<vector<bool>> strToBinaryImage(vector<string> each_image);

vector<int> getLabel(vector<string> label_vector);

map<int, long> countFrequency(vector<int> label_vector);

double getConditionalProb(vector<BinaryImage> binary_images, vector<int> label_vector,
                          map<int, long> frequency_map, int row, int column);
/**
 *
 * @return
 */
int main() {
    ifstream trainingStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/trainingimages");
    string line;
    vector<string> all_lines;

    if (trainingStream.is_open()) {
        while (getline(trainingStream, line)) {
            all_lines.push_back(line);
        }
    } else {
        perror("Training Images file is closed");
    }
    trainingStream.close();

    vector<vector<string>> all_images = eachImageVector(all_lines);

    vector<BinaryImage> all_binary_images;

    for (const auto &each_image : all_images) { //adding elements to vector<BinaryImage>
        vector<vector<bool>> b_vector = strToBinaryImage(each_image);
        BinaryImage each_binary_image(b_vector);
        all_binary_images.push_back(each_binary_image);
    }

    ifstream trainingLabelsStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/traininglabels");

    vector<string> label_vector;
    string image_value;
    if (trainingLabelsStream.is_open()) {
        while (getline(trainingLabelsStream, image_value)) {
            label_vector.push_back(image_value);
        }
    } else {
        perror("Training Labels file is closed");
    }
    trainingLabelsStream.close();

    vector<int> label_values = getLabel(label_vector); //converts string to int

    map<int, long> frequency_class = countFrequency(label_values); //counts frequency of int key

    vector<double> cond_prob_vector;
    //cond_prob_vector.reserve(CLASS_SIZE);

    int row = 1;
    int column = 1;

    for (int objIndex = 0; objIndex < all_binary_images.size(); objIndex++) {
        row = 1;
        column = 1;

        for (int lineIndex = row - 1; lineIndex < row; lineIndex++) {

            for (int boolIndex = column - 1; boolIndex < column; boolIndex++) {

                cout << (getConditionalProb(all_binary_images, label_values, frequency_class, row, column))
                     << '\n';

                if (column >= IMAGE_LENGTH) {
                    row++;
                    column = 1;

                } else {
                    column++;
                }
            }

            if (row >= (IMAGE_LENGTH - 1)) {
                break;

            } else {
                row++;
            }
        }
    }

    //cout << cond_prob_vector[0];
    return 0;
}

/**
 * Takes each line of file and creates a 2D string vector in which each the first index is an images
 * @param by_line
 * @return
 */
vector<vector<string>> eachImageVector(vector<string> all_lines) {
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
vector<vector<bool>> strToBinaryImage(vector<string> each_image) {
    vector<vector<bool>> binary_2d;
    binary_2d.resize(IMAGE_LENGTH - 1);

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
vector<int> getLabel(vector<string> label_vector) {
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
map<int, long> countFrequency(vector<int> label_vector) {
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
 * @param frequency_map
 */
double getConditionalProb(vector<BinaryImage> binary_images, vector<int> label_vector,
                            map<int, long> frequency_map, int row, int column) {
    int count = 0;

    for (int class_ = column - 1 ; class_ < CLASS_SIZE; class_++) {

    }

    long class_frequency;
    //= frequency_map.at(class_num);
    return (K_VALUE + count) / ((V * K_VALUE) + class_frequency);
}






