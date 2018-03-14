//
// Created by John Seol on 3/13/18.
//

#ifndef NAIVEBAYES_TRAINING_H
#define NAIVEBAYES_TRAINING_H

#include <vector>
#include <map>
#include "BinaryImage.h"

using namespace std;

class Training {
public:
    Training() = default;

    vector<vector<string>> eachImageVector(vector<string> by_line);

    vector<vector<bool>> strToBinaryImage(vector<string> each_image);

    vector<int> getLabel(vector<string> label_vector);

    map<int, long> countFrequency(vector<int> label_vector);

    vector<vector<int>> getForegroundCount(vector<BinaryImage> binary_images, vector<int> label_vector);

    vector<vector<int>> getBackgroundCount(vector<BinaryImage> binary_images, vector<int> label_vector);

    vector<vector<double>> getConditionalProbFor1(vector<vector<int>> count1_each_class, map<int, long> frequency_class);

    vector<vector<double>> getConditionalProbFor0(vector<vector<int>> count0_each_class, map<int, long> frequency_class);

    void makeConditionalProbFileFor1(vector<vector<double>> cond_prob_1);

    void makeConditionalProbFileFor0(vector<vector<double>> cond_prob_0);
};


#endif //NAIVEBAYES_TRAINING_H
