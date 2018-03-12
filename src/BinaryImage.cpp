//
// Created by John Seol on 3/12/18.
//

#include <iostream>
#include <vector>
#include "BinaryImage.h"

BinaryImage::BinaryImage() {
}

BinaryImage::BinaryImage(vector<vector<bool>> image) {
    binary_image = image;
}

vector<vector<bool>> BinaryImage::getBinaryImage() {
    return this->binary_image;
}