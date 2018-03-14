//
// Created by John Seol on 3/10/18.
//

#include "BinaryImage.h"
#include "Training.h"
#include "Classifying.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

bool file_is_empty(std::ifstream& training_model_file);

/**
 *
 * @return
 */
int main() {
    ifstream training_model_stream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities0");
    Training training;
    Classifying classifying;

    if (file_is_empty(training_model_stream)) { //if ConditionalProbabilities0 file is empty
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

        //all the training images as a 2d string vector:
        vector<vector<string>> all_images = training.eachImageVector(all_lines);

        vector<BinaryImage> all_binary_images;

        for (const auto &each_image : all_images) { //adding training images to vector<BinaryImage>
            vector<vector<bool>> b_vector = training.strToBinaryImage(each_image);
            BinaryImage each_binary_image(b_vector);
            all_binary_images.push_back(each_binary_image);
        }

        ifstream trainingLabelsStream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/traininglabels");

        vector<string> label_vector; //each line of traininglabels file
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

        map<int, long> frequency_class = training.countFrequency(label_values); //counts frequency of each class digit

        //counts each non-whitespace pixel of all the binary images:
        vector<vector<int>> count1_each_class = training.getForegroundCount(all_binary_images, label_values);

        //counts each whitespace pixel of all the binary images:
        vector<vector<int>> count0_each_class = training.getBackgroundCount(all_binary_images, label_values);

        //assigns the conditional probability of each pixel that aren't whitespaces of all the binary images
        vector<vector<double>> cond_prob_vector1 = training.getConditionalProbFor1(count1_each_class, frequency_class);

        //assigns the conditional probability of each pixel that are whitespaces of all the binary images
        vector<vector<double>> cond_prob_vector0 = training.getConditionalProbFor0(count0_each_class, frequency_class);

        //makes 2 files to store the counts:
        training.makeConditionalProbFileFor1(cond_prob_vector1);
        training.makeConditionalProbFileFor0(cond_prob_vector0);

    } else { //if file is not empty
        ifstream trainingModel1Stream(
                "/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities1");

        vector<string> all_cond1_str; //each line of ConditionalProbabilities1 file
        string each_cond_prob1;

        if (trainingModel1Stream.is_open()) {
            while (getline(trainingModel1Stream, each_cond_prob1)) {
                all_cond1_str.push_back(each_cond_prob1);
            }
        } else {
            perror("ConditionalProbabilities1 file is closed");
        }
        trainingModel1Stream.close();

        //assignes the vector the total count of all the non-whitespace pixels:
        vector<double> prob1_values = classifying.getProbValues(all_cond1_str);

        ifstream trainingModel0Stream(
                "/Users/johnseol/CLionProjects/naivebayes-jseol073/data/ConditionalProbabilities0");

        vector<string> all_cond0_str; //each line of ConditionalProbabilities0 file
        string each_cond_prob0;
        if (trainingModel0Stream.is_open()) {
            while (getline(trainingModel0Stream, each_cond_prob0)) {
                all_cond0_str.push_back(each_cond_prob0);
            }
        } else {
            perror("ConditionalProbabilities0 file is closed");
        }
        trainingModel0Stream.close();

        //assignes the vector the total count of all the whitespace pixels:
        vector<double> prob0_values = classifying.getProbValues(all_cond0_str);

        vector<vector<double>> prob0_class_values = classifying.getTrainingProb(prob0_values); //no error

        vector<vector<double>> prob1_class_values = classifying.getTrainingProb(prob1_values); //no error

        ifstream test_images_file_stream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/testimages");
        vector<string> test_all_lines = classifying.getFileLines(test_images_file_stream);
        test_images_file_stream.close();

        vector<vector<string>> test_images_2d = training.eachImageVector(test_all_lines);
        vector<BinaryImage> all_binary_test_images;

        for (const auto &each_image : test_images_2d) { //adding test images to all_binary_test_images
            vector<vector<bool>> b_vector = training.strToBinaryImage(each_image);
            BinaryImage each_binary_image(b_vector);
            all_binary_test_images.push_back(each_binary_image);
        }

        ifstream training_labels_stream("/Users/johnseol/CLionProjects/naivebayes-jseol073/data/traininglabels");

        //training labels to vector<string> :
        vector<string> label_vector = classifying.getFileLines(training_labels_stream);
        training_labels_stream.close();

        vector<int> label_values = training.getLabel(label_vector); //converts string to int

        map<int, long> frequency_classes = training.countFrequency(label_values); //counts frequency of each class digit

        vector<double> post_probs = classifying.calculatePosteriorProbs
                (all_binary_test_images, prob0_class_values, prob1_class_values, frequency_classes);
    }
    training_model_stream.close();
    return 0;
}

/**
 * Got code from https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
 * Checks if a file is empty
 * @param training_model_file, refers to ConditionalProbability file
 * @return true if file is empty, false otherwise
 */
bool file_is_empty(ifstream& training_model_file) {
    return training_model_file.peek() == ifstream::traits_type::eof();
}






