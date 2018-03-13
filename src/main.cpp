//
// Created by John Seol on 3/10/18.
//

#include "BinaryImage.h"
#include "Training.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

bool is_empty(std::ifstream& pFile);

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
        perror("trainingimages file is closed");
    }
    trainingStream.close();

    Training training;

    vector<vector<string>> all_images = training.eachImageVector(all_lines);

    vector<BinaryImage> all_binary_images;

    for (const auto &each_image : all_images) { //adding elements to vector<BinaryImage>
        vector<vector<bool>> b_vector = training.strToBinaryImage(each_image);
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
        perror("traininglabels file is closed");
    }
    trainingLabelsStream.close();

    vector<int> label_values = training.getLabel(label_vector); //converts string to int

    map<int, long> frequency_class = training.countFrequency(label_values); //counts frequency of int key

    vector<vector<int>> count1_each_class = training.getForegroundCount(all_binary_images, label_values);

    vector<vector<int>> count0_each_class = training.getBackgroundCount(all_binary_images, label_values);

    vector<vector<double>> cond_prob_vector1 = training.getConditionalProbFor1(count1_each_class, frequency_class);

    vector<vector<double>> cond_prob_vector0 = training.getConditionalProbFor0(count0_each_class, frequency_class);

    training.makeConditionalProbFile(cond_prob_vector1);


    ifstream trainingModelStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities");

    vector<string> all_cond_str;
    string each_cond_prob;
    if (trainingModelStream.is_open()) {
        while (getline(trainingModelStream, each_cond_prob)) {
            all_cond_str.push_back(each_cond_prob);
        }
    } else {
        perror("traininglabels file is closed");
    }
    trainingLabelsStream.close();

    return 0;
}

/**
 * Got code from https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
 * @param pFile
 * @return
 */
bool is_empty(std::ifstream& pFile) {
    return pFile.peek() == ifstream::traits_type::eof();
}






