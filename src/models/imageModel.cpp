// Dependencies
#include <models/imageModel.h>

// Getters
short ImageModel::getWidth() {
    return width;
}
short ImageModel::getHeight() {
    return height;
}
unsigned char** ImageModel::getMatrix() {
    return matrix;
}

// Setters
void ImageModel::setWidth(short w){
    width = w;
}
void ImageModel::setHeight(short h){
    height = h;
}
void ImageModel::setMatrix(unsigned char** m){
    matrix = m;
}
