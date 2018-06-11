//
// Created by John Seol on 3/13/18.
//

#include "Classifying.h"
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
#include <math.h>

using namespace std;

const int CLASS_SIZE = 10;

const int CLASS_BORDER = 756;

const int TRAINING_IMAGES_SIZE = 5000;

Training training;

/**
 * takes a vector<string> and converts each element into a double
 * @param prob_vector, a vector<string> that contains the lines of either ConditionalProbability1 and
 * ConditionalProbability0 files
 * @return a vector<double> which is the same but each element is now in double
 */
vector<double> Classifying::getProbValues(vector<string> prob_vector) {
    vector<double> prob_values;
    for (const auto &label : prob_vector) {
        stringstream num(label);
        double each_value = 0;
        num >> each_value;
        prob_values.push_back(each_value);
    }
    return prob_values;
}

/**
 * Takes all_lines and makes a 2d vector in which the first index refers to class digit index and the second
 * index refers to the the lines of the probabilities
 * @param all_lines, refers to the lines of the file with all the conditional porbabilities
 * @return a 2d vector in which the first index refers to class digit index and the second
 * index refers to the the lines of the probabilities
 */
vector<vector<double>> Classifying::getTrainingProb(vector<double> all_lines) {
    vector<vector<double>> prob_classes;
    prob_classes.resize(CLASS_SIZE);
    int class_index = 0;

    for (int line_index = 0; line_index < all_lines.size(); line_index++) {
        if (line_index != 0 && line_index % CLASS_BORDER == 0) {
            class_index++;
        } else {
            prob_classes[class_index].push_back(all_lines[line_index]);
        }
    }
    return prob_classes;
}

/**
 * Takes a file and returns a vector<string> which contains each line of the file
 * @param test_images_file_stream, refers to the filename
 * @return a vector<string> which contains each line of the file
 */
vector<string> Classifying::getFileLines(ifstream& test_images_file_stream) {
    vector<string> all_lines;
    string line;

    if (test_images_file_stream.is_open()) {
        while (getline(test_images_file_stream, line)) {
            all_lines.push_back(line);
        }
    } else {
        perror("this file is closed");
    }
    return all_lines;
}

/**
 * This method iterates through every BinaryImage and each iteration it calls getMaxPosteriorProb method
 * @param all_binary_images, all the test images in binary
 * @param prob0_values prior probability values from the ConditionalProbability0
 * @param prob1_values, prior probability values from the ConditionalProbability1
 * @param objIndex, the index referring to the current BinaryImage objects of when this method is called
 * @param frequency_class, a map that contains the frequency of each class digit
 * @return a vecter<double> which is size 10 and contains the max posterior probability for each class
 */
vector<double> Classifying::calculatePosteriorProbs
        (vector<BinaryImage> all_binary_images, vector<vector<double>> prob0_values,
         vector<vector<double>> prob1_values, map<int, long> frequency_class) {
    vector<double> post_probs;

    for (int objIndex = 0; objIndex < all_binary_images.size(); objIndex++) {
        post_probs.clear();
        post_probs.push_back(getMaxPosteriorProb(all_binary_images, prob0_values,
                                                       prob1_values, objIndex, frequency_class));
    }
    return post_probs;
}

/**
 * helper method for calculatePosteriorProbs
 * When called, this method iterates through the pixels of test image and then calculates bayes theorem
 * for each class digit and returns the largest posterior probability out of all the values from classes
 * @param all_binary_images, all the test images in binary
 * @param prob0_values, prior probability values from the ConditionalProbability0
 * @param prob1_values, prior probability values from the ConditionalProbability1
 * @param objIndex, the index referring to the current BinaryImage objects of when this method is called
 * @param frequency_class, a map that contains the frequency of each class digit
 * @return returns the highest probability (of all the classes) for this current image
 */
double Classifying::getMaxPosteriorProb
        (vector<BinaryImage> all_binary_images, vector<vector<double>> prob0_values,
         vector<vector<double>> prob1_values, int objIndex, map<int, long> frequency_class) {
    vector<double> post_probs_by_class;
    int prior_prob_index = 0;
    vector<double> probForEachClass;

    for (int class_ = 0; class_ < CLASS_SIZE; class_++) {
        //double sum_of_probs = 0;
        double prob_of_class = ((frequency_class.at(class_)) / TRAINING_IMAGES_SIZE);
        probForEachClass.push_back(log(prob_of_class));

        for (int line_index = 0; line_index < all_binary_images[objIndex].getBinaryImage().size(); line_index++) {

            for (int bool_index = 0; bool_index <
                                     all_binary_images[objIndex].getBinaryImage()[line_index].size(); bool_index++) {

                if (all_binary_images[objIndex].getBinaryImage()[line_index][bool_index]) {

                    if (prior_prob_index >= CLASS_BORDER) {
                        break;

                    } else {
                        probForEachClass.push_back(log(prob1_values[class_][prior_prob_index]));
                        prior_prob_index++;
                    }
                } else {
                    if (prior_prob_index >= CLASS_BORDER) {
                        break;

                    } else {
                        probForEachClass.push_back(log(prob0_values[class_][prior_prob_index]));
                        prior_prob_index++;
                    }
                }
            }
        }
        double sum = getSumOfVector(probForEachClass);
        post_probs_by_class.push_back(sum);
    }

    //returning highest probability (of all the classes) for this current image
    auto max_prob = max_element(post_probs_by_class.begin(), post_probs_by_class.end());
    return *max_prob;
}

double Classifying::getSumOfVector(vector<double> probVector) {
    double sum = 0;
    for (double prob : probVector) {
        sum += prob;
    }
    return sum;
}