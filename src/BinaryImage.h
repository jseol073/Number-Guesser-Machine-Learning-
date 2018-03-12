//
// Created by John Seol on 3/12/18.
//

#ifndef NAIVEBAYES_BINARYIMAGE_H
#define NAIVEBAYES_BINARYIMAGE_H

#include <vector>

using namespace std;

class BinaryImage {
private:
    vector<vector<bool>> binary_image;
public:
    BinaryImage();
    BinaryImage(vector<vector<bool>> image);
    vector<vector<bool>> getBinaryImage();
};


#endif //NAIVEBAYES_BINARYIMAGE_H
