//
// Created by John Seol on 3/13/18.
//

#ifndef NAIVEBAYES_CLASSIFYING_H
#define NAIVEBAYES_CLASSIFYING_H

#include "BinaryImage.h"
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

class Classifying {
public:
    Classifying() = default;
    vector<double> getProbValues(vector<string> prob_vector);
    vector<vector<double>> getTrainingProb(vector<double> all_lines);
    vector<string> getFileLines(ifstream& test_images_file_stream);
    vector<double> calculatePosteriorProbs(vector<BinaryImage> all_binary_images, vector<vector<double>> prob0_values,
                                           vector<vector<double>> prob1_values, map<int, long> frequency_class);
    double getMaxPosteriorProb(vector<BinaryImage> all_binary_images, vector<vector<double>> prob0_values,
                                             vector<vector<double>> prob1_values, int objIndex, map<int, long> frequency_class);
    double getSumOfVector(vector<double> probVector);
};


#endif //NAIVEBAYES_CLASSIFYING_H
