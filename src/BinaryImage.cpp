//
// Created by John Seol on 3/12/18.
//

#include <iostream>
#include <vector>
#include "BinaryImage.h"

BinaryImage::BinaryImage() {
}

/**
 * this object just contains a 2d bool vector that represents the binary image
 * @param image
 */
BinaryImage::BinaryImage(vector<vector<bool>> image) {
    binary_image = image;
}

/**
 * getter to access 2d bool vector
 * @return
 */
vector<vector<bool>> BinaryImage::getBinaryImage() {
    return binary_image;
}