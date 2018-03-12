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

vector<vector<string>> eachImageVector(vector<string> by_line);

//vector<vector<vector<bool>>> binaryImages(vector<vector<string>> all_images);

vector<vector<bool>> strToBinaryImage(vector<string> each_image);

vector<int> getLabel(vector<string> label_vector);

int main() {
    ifstream trainingStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/trainingimages");
    string line;
    vector<string> all_lines;

    if (trainingStream.is_open()) {
        while (getline(trainingStream, line)) {
            all_lines.push_back(line);
        }
    } else {
        perror("File is closed");
    }
    trainingStream.close();

    vector<vector<string>> all_images = eachImageVector(all_lines);

    vector<BinaryImage> all_binary_images;
    all_binary_images.resize(all_images.size());

    for (auto each_image : all_images) { //adding elements to vector<BinaryImage>
        each_image.resize(IMAGE_LENGTH);
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
        perror("File is closed");
    }

    trainingLabelsStream.close();
    cout << label_vector[1] << ", ";
    return 0;
}

/**
 * Takes each line of file and creates a 2D string vector in which each the first index is an images
 * @param by_line
 * @return
 */
vector<vector<string>> eachImageVector(vector<string> all_lines) {
    vector<vector<string>> all_images;
    all_images.resize((all_lines.size() / IMAGE_LENGTH) + 1);

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

//vector<vector<vector<bool>>> binaryImages(vector<vector<string>> all_images) {
//    vector<vector<vector<bool>>> binary_images;
//    binary_images.resize(all_images.size());
//
//    for (int image_index = 0; image_index < all_images.size(); image_index++) {
//        binary_images[image_index].resize(all_images[image_index].size());
//
//        for (int line_index = 0; line_index < all_images[image_index].size(); line_index++) {
//            binary_images[image_index][line_index].resize(IMAGE_LENGTH);
//
//            for (int pos = 0; pos < IMAGE_LENGTH; pos++) {
//                if (all_images[image_index][line_index].at(pos) == '+') {
//                    binary_images[image_index][line_index][pos] = true;
//
//                } else if (all_images[image_index][line_index].at(pos) == '#') {
//                    binary_images[image_index][line_index][pos] = true;
//
//                } else if (all_images[image_index][line_index].at(pos) == ' ') {
//                    binary_images[image_index][line_index][pos] = false;
//                }
//            }
//        }
//    }
//    return binary_images;
//}

/**
 * Takes each line of file and makes a 2D boolean vector
 * @param each_line
 * @return
 */
vector<vector<bool>> strToBinaryImage(vector<string> each_image) {
    vector<vector<bool>> binary_2d;
    binary_2d.resize(IMAGE_LENGTH - 1);

    for (int line_index = 0; line_index < each_image.size() - 1; line_index++) {
        binary_2d[line_index].resize(IMAGE_LENGTH);

        if (!each_image[line_index].empty()) {
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

vector<int> getLabels(vector<string> label_vector) {
    vector<int> int_labels;
    for (auto label : label_vector) {
        stringstream num(label);
        int image_value = 0;
        num >> image_value;
        int_labels.push_back(image_value);
    }
    return int_labels;
}






